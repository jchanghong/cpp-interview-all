#!/usr/bin/env python3
"""Convert all cppinter markdown files into a single EPUB ebook."""

import markdown
import re
from ebooklib import epub
from pathlib import Path

BASE = Path("/root/cppinter")
OUTPUT = BASE / "cppinter_面试指南.epub"

# ── files to include (in order) ──────────────────────────────────────────
# Skip AGENTS.md (AI agent instructions, not interview content)
CHAPTERS = [
    # (filename, title, type)
    ("README.md", "封面与简介", "cover"),
    ("ch01_编程语言基础.md", "第1章 编程语言基础", "chapter"),
    ("ch03_操作系统与Linux.md", "第2章 操作系统与Linux", "chapter"),
    ("ch04_数据库与缓存.md", "第3章 数据库与缓存", "chapter"),
    ("ch05_数据结构与算法.md", "第4章 数据结构与算法", "chapter"),
    ("ch06_算法刷题题解.md", "第5章 算法刷题题解", "chapter"),
    ("ch07_设计模式与项目实践.md", "第6章 设计模式与项目实践", "chapter"),
    ("ch08_系统设计与分布式.md", "第7章 系统设计与分布式", "chapter"),
    ("ch09_AI与LLM面试追问.md", "第8章 AI与LLM面试追问", "chapter"),
    ("mini_all.md", "附录 完整合并版", "appendix"),
]


def md_to_html(md_text: str) -> str:
    """Convert markdown to clean HTML body content."""
    extensions = [
        "markdown.extensions.fenced_code",
        "markdown.extensions.tables",
        "markdown.extensions.codehilite",
        "markdown.extensions.toc",
    ]
    html = markdown.markdown(md_text, extensions=extensions)

    # Fix heading IDs — remove URL-encoded and weird characters
    html = re.sub(r'id="[^"]*%[^"]*"', 'id=""', html)

    # Wrap in a nice div
    return html


def build_epub():
    """Main build function."""
    book = epub.EpubBook()

    # ── metadata ──
    book.set_identifier("cppinter-interview-guide-2024")
    book.set_title("C++ 社招面试指南")
    book.set_language("zh")
    book.add_author("cppinter 社区")
    book.add_metadata("DC", "description",
                      "C++ 社招面试准备资料库，涵盖编程语言基础、操作系统、数据库、"
                      "数据结构与算法、设计模式、系统设计、AI/LLM等主题。")

    # ── CSS ──
    style = """
    body { font-family: "Noto Sans CJK SC", "Source Han Sans CN", "PingFang SC",
           "Microsoft YaHei", sans-serif; line-height: 1.8; margin: 1em 2em;
           color: #333; }
    h1 { border-bottom: 2px solid #2c3e50; padding-bottom: 0.3em; color: #2c3e50; }
    h2 { color: #34495e; margin-top: 1.5em; }
    h3 { color: #555; }
    pre { background: #f4f4f4; border: 1px solid #ddd; border-radius: 4px;
          padding: 1em; overflow-x: auto; font-size: 0.9em; }
    code { background: #f0f0f0; padding: 0.15em 0.3em; border-radius: 3px;
           font-size: 0.9em; }
    pre code { background: none; padding: 0; }
    blockquote { border-left: 4px solid #3498db; margin: 1em 0; padding: 0.5em 1em;
                 background: #eef7fb; color: #555; }
    table { border-collapse: collapse; width: 100%; margin: 1em 0; }
    th, td { border: 1px solid #ccc; padding: 0.5em; text-align: left; }
    th { background: #f7f7f7; }
    hr { border: none; border-top: 1px dashed #ccc; margin: 2em 0; }
    """
    css = epub.EpubItem(uid="style", file_name="style/default.css",
                        media_type="text/css", content=style.encode("utf-8"))
    book.add_item(css)

    # ── process each chapter ──
    spine = ["nav"]
    toc_entries = []

    for idx, (fname, title, ctype) in enumerate(CHAPTERS, start=1):
        fpath = BASE / fname
        if not fpath.exists():
            print(f"  [SKIP] {fname} — not found")
            continue

        md_text = fpath.read_text(encoding="utf-8")

        # Clean up broken relative links (per AGENTS.md rules)
        md_text = re.sub(r'\]\(problems__[^)]*\)', '](见原文档)', md_text)
        md_text = re.sub(r'\]\(docs__[^)]*\)', '](见原文档)', md_text)
        md_text = re.sub(r'\]\(contents__[^)]*\)', '](见原文档)', md_text)

        body_html = md_to_html(md_text)

        # Wrap in proper XHTML
        xhtml = f"""<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="zh">
<head>
  <title>{title}</title>
  <link rel="stylesheet" type="text/css" href="style/default.css"/>
</head>
<body>
<h1>{title}</h1>
{body_html}
</body>
</html>"""

        file_id = f"ch{idx:02d}"
        file_name = f"text/{file_id}.xhtml"

        chapter = epub.EpubHtml(title=title, file_name=file_name, lang="zh")
        chapter.content = xhtml.encode("utf-8")
        chapter.add_item(css)

        book.add_item(chapter)
        spine.append(chapter)

        # For cover page, don't number it as a chapter in TOC style
        toc_entries.append(epub.Link(file_name, title, file_id))
        book.toc.append(toc_entries[-1])

        print(f"  [OK] {fname} → {file_name}")

    # ── NAV (required for EPUB3) ──
    book.add_item(epub.EpubNcx())
    book.add_item(epub.EpubNav())

    # ── spine order ──
    book.spine = spine

    # ── write ──
    epub.write_epub(str(OUTPUT), book)
    print(f"\n✅ EPUB written to: {OUTPUT}")


if __name__ == "__main__":
    build_epub()
