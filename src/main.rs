use ariadne::{CharSet, Color, Config, Label, Report, ReportKind, Source};
use chumsky::{container::Container, prelude::*};
use core::fmt;
use mlua::prelude::*;
use std::{
    collections::{HashMap, LinkedList},
    process::Child,
};
use tagu::elem::Element;

mod html {
    use ammonia::clean_text;
    use lazy_static::lazy_static;
    use mlua::prelude::*;
    use mlua::UserData;
    use std::any::Any;
    use std::any::TypeId;
    use std::{
        collections::HashSet,
        fmt::{write, Display},
    };
    use tagu::elem;

    lazy_static! {
        static ref VOID: HashSet<String> = HashSet::from(
            [
                "area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta",
                "param", "source", "track", "wbr",
            ]
            .map(ToOwned::to_owned)
        );
    }

    #[derive(Debug)]
    pub struct Text(ecow::EcoString);

    impl Text {
        pub fn new(text: impl AsRef<str>) -> Self {
            Text(text.as_ref().into())
        }
    }

    impl Display for Text {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(f, "{}", clean_text(&self.0))
        }
    }

    #[derive(Debug)]
    pub enum Child {
        Text(Text),
        Element(Element),
    }

    impl From<Element> for Child {
        fn from(value: Element) -> Self {
            Self::Element(value)
        }
    }

    impl From<Text> for Child {
        fn from(value: Text) -> Self {
            Self::Text(value)
        }
    }

    impl From<&str> for Child {
        fn from(value: &str) -> Self {
            Self::Text(Text::new(value))
        }
    }

    impl Display for Child {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            match self {
                Child::Text(text) => write!(f, "{text}"),
                Child::Element(elem) => write!(f, "{elem}"),
            }
        }
    }

    #[derive(Debug)]
    pub struct Attrs(Vec<(ecow::EcoString, ecow::EcoString)>);

    #[derive(Debug)]
    pub struct Element {
        tag: ecow::EcoString,
        attributes: Attrs,
        child_list: Vec<Child>,
    }

    impl Element {
        pub fn new(tag: impl AsRef<str>) -> Self {
            Self {
                tag: tag.as_ref().into(),
                attributes: Attrs(Vec::new()),
                child_list: Vec::new(),
            }
        }

        pub fn push(mut self, child: impl Into<Child>) -> Self {
            self.child_list.push(child.into());
            return self;
        }

        pub fn push_mut(&mut self, child: impl Into<Child>) {
            self.child_list.push(child.into());
        }
    }

    impl UserData for Element {
        fn add_fields<'lua, F: LuaUserDataFields<'lua, Self>>(_: &mut F) {}

        fn add_methods<'lua, M: LuaUserDataMethods<'lua, Self>>(methods: &mut M) {
            methods.add_method_mut("push", |_, this, value: LuaValue| {
                this.push_mut(match value {
                    LuaValue::UserData(any) => Child::Element(any.take::<Element>()?),
                    LuaValue::String(text) => Child::Text(Text::new(text.to_str()?)),
                    _ => todo!(),
                });
                Ok(())
            });

            methods.add_meta_method(LuaMetaMethod::ToString, |_, this, _: ()| {
                Ok(format!("{this}"))
            })
        }
    }

    impl Display for Attrs {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            for (attr, value) in &self.0 {
                write!(f, "{attr}=\"{value}\" ", value = clean_text(&value))?
            }

            Ok(())
        }
    }

    impl Display for Element {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            let Element {
                tag,
                attributes,
                child_list,
            } = self;

            if VOID.contains(tag.as_ref()) {
                write!(f, "<{tag}{attributes}/>")?;
            } else {
                write!(f, "<{tag}{attributes}>")?;

                for child in child_list {
                    write!(f, "{child}")?;
                }

                write!(f, "<{tag}/>")?;
            }

            Ok(())
        }
    }
}

#[derive(Clone)]
enum Inline {
    Word(EcoString),
    Space(EcoString),
    Symbol(EcoString),
    Math(EcoString),
    Code { lang: String, src: EcoString },
    Newline,
    End,
}

impl fmt::Debug for Inline {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Inline::Word(a) => write!(f, "Word({a:?})"),
            Inline::Space(a) => write!(f, "Space({a:?})"),
            Inline::Symbol(a) => write!(f, "Symbol({a:?})"),
            Inline::Math(a) => write!(f, "Math({a:?})"),
            Inline::Newline => write!(f, "Newline"),
            Inline::End => write!(f, "End"),
            Inline::Code { lang, src } => todo!(),
        }
    }
}

#[derive(Default, Clone)]
struct EcoString(pub ecow::EcoString);

impl fmt::Debug for EcoString {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:?}", self.0)
    }
}

impl Container<char> for EcoString {
    fn push(&mut self, item: char) {
        self.0.push(item)
    }
}

#[derive(Debug, Clone)]
struct Doc(Option<Func>, Vec<Block>);

#[derive(Clone)]
struct Func(EcoString);

impl fmt::Debug for Func {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "λ {}", self.0 .0)
    }
}

impl Func {
    fn new(s: ecow::EcoString) -> Self {
        Func(EcoString({
            let mut chars = s.chars();
            chars.next();
            chars.next_back();
            chars.into_iter().collect()
        }))
    }
}

#[derive(Debug, Clone)]
enum Block {
    Par(Vec<Inline>),
    List(Vec<Doc>),
    Func(Func),
    Raw(String),
}

fn merge(mut head: Vec<Inline>, mut tail: LinkedList<Block>) -> LinkedList<Block> {
    let block = match tail.pop_front() {
        Some(Block::Par(ref mut lines)) => {
            head.append(lines);
            Block::Par(head)
        }
        Some(non_list) => {
            tail.push_front(non_list);
            Block::Par(head)
        }
        None => Block::Par(head),
    };

    tail.push_front(block);
    tail
}

fn parser<'a>() -> impl Parser<'a, &'a str, Doc, extra::Err<Rich<'a, char>>> {
    let space = || just(' ').repeated().at_least(1).collect();
    let nl = || just('\n').to(Inline::Newline);
    let end = || end().to(Inline::End);
    let blank = || space().or_not().then(nl());

    let indent = just(' ')
        .repeated()
        .configure(|repeat, current_indent: &usize| repeat.at_least(*current_indent));

    let word = || {
        any()
            .filter(|ch: &char| ch.is_alphanumeric())
            .repeated()
            .at_least(1)
            .collect()
    };

    let symbol = || {
        any()
            .and_is(space().not())
            .and_is(nl().not())
            .repeated()
            .at_least(1)
            .collect()
    };

    let math = || {
        any()
            .and_is(just('$').not())
            .repeated()
            .collect()
            .delimited_by(just("$"), just("$"))
    };

    let inline = || {
        choice((
            word().map(Inline::Word),
            space().map(Inline::Space),
            math().map(Inline::Math),
            symbol().map(Inline::Symbol),
        ))
    };

    let raw = || {
        any()
            .and_is(nl().not())
            .repeated()
            .collect::<String>()
            .delimited_by(group((indent, just('|'))), nl().or(end()))
            .repeated()
            .at_least(1)
            .collect::<Vec<_>>()
            .map(|lines| lines.join("\n"))
            .delimited_by(blank().repeated(), just(""))
    };

    let func = || {
        recursive(|paren| {
            group((
                just('('),
                none_of("()").repeated().collect::<String>(),
                paren
                    .then(none_of("()").repeated().collect::<String>())
                    .map(|(a, b)| format!("{a}{b}"))
                    .repeated()
                    .collect::<Vec<String>>(),
                none_of("()").repeated().collect::<String>(),
                just(')'),
            ))
            .map(|(_, a, inner, b, _)| {
                let inner = inner.join("");
                format!("({a}{inner}{b})")
            })
        })
        .delimited_by(group((blank().repeated(), indent, just('#'))), just(""))
        .map(ecow::EcoString::from)
    };

    let line = || {
        group((
            inline().repeated().at_least(1).collect::<Vec<_>>(),
            nl().or(end()),
        ))
        .map(|(mut xs, x)| {
            xs.push(x);
            xs
        })
        .delimited_by(indent, just(""))
    };

    let par = || {
        blank().repeated().ignore_then(
            (line().repeated().at_least(1))
                .collect::<Vec<Vec<Inline>>>()
                .map(|v| v.into_iter().flatten().collect::<Vec<_>>()),
        )
    };

    let mut block = Recursive::declare();

    let bullet = || just('-').then(space().or_not());

    let list_item = || {
        (func().then_ignore(nl()).or_not())
            .then((indent.count()).ignore_with_ctx(group((
                bullet().ignore_then(line().or_not().with_ctx(0)),
                blank().ignored().or_not(),
                map_ctx(
                    |indent: &usize| *indent + 1,
                    block.clone().repeated().collect::<LinkedList<Block>>(),
                ),
            ))))
            .map(|(f, (head, gap, mut tail))| {
                let Some(head) = head else {
                    return Doc(f.map(Func::new), tail.into_iter().collect::<Vec<Block>>());
                };

                Doc(
                    f.map(Func::new),
                    if gap.is_some() {
                        tail.push_front(Block::Par(head));
                        tail.into_iter().collect()
                    } else {
                        merge(head, tail).into_iter().collect::<Vec<Block>>()
                    },
                )
            })
            .delimited_by(blank().repeated(), just(""))
    };

    let list = || list_item().repeated().at_least(1).collect::<Vec<_>>();

    block.define(choice((
        list().map(Block::List),
        func().map(Func::new).map(Block::Func),
        raw().map(Block::Raw),
        par().map(Block::Par),
    )));

    block
        .repeated()
        .collect::<Vec<Block>>()
        .map(|a| Doc(None, a))
        .delimited_by(blank().repeated(), blank().repeated())
        .then_ignore(end())
        .with_ctx(0)
}

fn render_doc(lua: &Lua, doc: Doc, depth: usize) -> LuaResult<()> {
    for block in doc.1.into_iter() {
        match block {
            Block::Par(inlines) => {
                for inline in inlines {
                    // println!("{inline:?}");
                }
            }
            Block::List(list) => {
                for doc in list {
                    if let Some(Func(ref chunk)) = doc.0 {
                        lua.load(chunk.0.to_string()).exec()?;
                    }
                    render_doc(&lua, doc, depth + 1);
                }
            }
            Block::Func(Func(EcoString(chunk))) => {
                lua.load(chunk.to_string()).exec()?;
            }
            Block::Raw(raw) => {
                // println!("{raw:?}")
            }
        }
    }
    Ok(())
}

#[derive(thiserror::Error)]
enum Error {
    #[error(transparent)]
    Lua(#[from] mlua::Error),
}

impl fmt::Debug for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Error::Lua(LuaError::RuntimeError(err)) => write!(f, "\n{}", err),
            err => write!(f, "{err}"),
        }
    }
}

type Result<T, E = Error> = std::result::Result<T, E>;

fn main() -> Result<()> {
    let lua = Lua::new_with(LuaStdLib::ALL_SAFE, LuaOptions::default())?;

    lua.globals().set(
        "metadata",
        lua.create_function(|lua: &Lua, table: LuaTable| -> LuaResult<LuaFunction> {
            println!("metadata {:?}", table);
            lua.create_function(|_: &Lua, _: ()| Ok(println!("hello!")))
        })?,
    )?;

    lua.globals().set(
        "question",
        lua.create_function(|_lua: &Lua, table: LuaTable| {
            println!("question {:?}", table);
            Ok(())
        })?,
    )?;

    lua.globals().set(
        "html",
        HashMap::from([(
            "node",
            lua.create_function(
                |lua: &Lua, (tag, _attrs, child_list): (LuaString, LuaTable, Vec<LuaValue>)| {
                    let mut elem = Element::new(tag.to_str()?);
                    for child in child_list {
                        match child {
                            LuaValue::String(text) => elem.push_mut(text.to_str()?),
                            LuaValue::UserData(any) => elem.push_mut(any.take::<Element>()?),
                            _ => todo!(),
                        }
                    }
                    Ok(elem)
                },
            )?,
        )]),
    )?;

    lua.globals().set(
        "math",
        lua.create_function(|_: &Lua, table: LuaTable| {
            println!("math {:?}", table);
            Ok(())
        })?,
    )?;

    lua.globals().set(
        "image",
        HashMap::from([(
            "invertable",
            lua.create_function(|_: &Lua, table: mlua::Table| {
                Ok(println!("image {:?}!", table.get::<_, LuaValue>("width")))
            })?,
        )]),
    )?;

    let text = include_str!("../test.tupi");

    let result = parser().parse(text).into_result();
    match result {
        Ok(ref a) => {
            dbg!(a);
        }
        Err(ref errs) => {
            for err in errs {
                Report::build(ReportKind::Error, (), err.span().start)
                    .with_code(3)
                    .with_message(err.to_string())
                    .with_label(
                        Label::new(err.span().into_range())
                            .with_message(err.reason().to_string())
                            .with_color(Color::Red),
                    )
                    .with_config(Config::default().with_char_set(CharSet::Ascii))
                    .finish()
                    .eprint(Source::from(text))
                    .unwrap();
            }
        }
    };

    render_doc(&lua, result.unwrap(), 0)?;

    use html::*;

    println!(
        "{}",
        Element::new("html").push(Element::new("p"))
    );

    Ok(())
}
