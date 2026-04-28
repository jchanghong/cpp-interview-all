#!/usr/bin/env python3
"""
Merge LeetBook and 剑指Offer algorithm sections, keeping only C++ versions.
Fixed version with correct boundary detection.
"""

import re

def read_file(filepath):
    """Read the file into a list of lines."""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.readlines()

def write_file(filepath, lines):
    """Write lines to file."""
    with open(filepath, 'w', encoding='utf-8') as f:
        f.writelines(lines)

def remove_lcr_and_丑数(lines):
    """Remove the LCR section and 丑数 combined section."""
    # Find boundaries
    lcr_start = None
    丑数_start = None
    丑数_end = None

    for i, line in enumerate(lines):
        if line.startswith('## LCR 120. 寻找文件副本'):
            lcr_start = i
        if line.startswith('## 剑指 Offer 49、LCR 168、264. 丑数'):
            丑数_start = i
        if 丑数_start is not None and line.startswith('## 10. 正则表达式匹配'):
            丑数_end = i
            break

    print(f"Found LCR section: lines {lcr_start} to {丑数_start}")
    print(f"Found 丑数 section: lines {丑数_start} to {丑数_end}")

    if lcr_start is not None and 丑数_end is not None:
        new_lines = lines[:lcr_start] + lines[丑数_end:]
        print(f"Removed {丑数_end - lcr_start} lines")
        return new_lines
    else:
        print("Could not find section boundaries")
        return lines

def remove_overlapping_leetcode(lines):
    """Remove LeetCode problems that overlap with 剑指Offer."""
    # LeetCode problems that are EXACTLY the same as 剑指Offer problems
    overlapping_leetcode = {
        '10. 正则表达式匹配',
        '101. 对称二叉树',
        '102. 二叉树的层序遍历',
        '103. 二叉树的锯齿形层序遍历',
        '104. 二叉树的最大深度',
        '105. 从前序与中序遍历序列构造二叉树',
        '110. 平衡二叉树',
        '121. 买卖股票的最佳时机',
        '138. 复制带随机指针的链表',
        '155. 最小栈',
        '160. 相交链表',
        '191. 位1的个数',
        '206. 反转链表',
        '21. 合并两个有序链表',
        '226. 翻转二叉树',
        '232. 用栈实现队列',
        '233. 数字 1 的个数',
        '235. 二叉搜索树的最近公共祖先',
        '236. 二叉树的最近公共祖先',
        '237. 删除链表中的节点',
        '239. 滑动窗口最大值',
        '295. 数据流中的中位数',
        '297. 二叉树的序列化与反序列化',
        '3. 无重复字符的最长子串',
        '343. 整数拆分',
        '371. 两整数之和',
        '387. 字符串中的第一个唯一字符',
        '400. 第 N 位数字',
        '426. 将二叉搜索树转化为排序的双向链表',
        '50. Pow(x, n)',
        '509. 斐波那契数',
        '53. 最大子数组和',
        '54. 螺旋矩阵',
        '59. 螺旋矩阵 II',
        '64. 最小路径和',
        '65. 有效数字',
        '70. 爬楼梯',
        '8. 字符串转换整数 (atoi)',
        '946. 验证栈序列',
    }

    new_lines = []
    i = 0
    removed_count = 0

    while i < len(lines):
        line = lines[i]

        # Check if this is a LeetCode problem header (before 剑指Offer section)
        match = re.match(r'^## (\d+)\. (.+)$', line)
        if match:
            problem_num = match.group(1)
            problem_title = match.group(2).strip()
            problem_key = f"{problem_num}. {problem_title}"

            if problem_key in overlapping_leetcode:
                print(f"Removing overlapping LeetCode problem: {problem_key}")
                removed_count += 1
                # Skip this problem and all its content
                i += 1
                while i < len(lines):
                    # Stop at next problem header or 剑指Offer section
                    if lines[i].startswith('## ') and (
                        re.match(r'^## \d+\.', lines[i]) or
                        lines[i].startswith('## 剑指 Offer') or
                        lines[i].startswith('## 设计模式')
                    ):
                        break
                    i += 1
                continue

        new_lines.append(line)
        i += 1

    print(f"Removed {removed_count} overlapping LeetCode problems")
    return new_lines

def strip_python_java(lines):
    """Remove Python and Java code blocks, keeping only C++."""
    new_lines = []
    i = 0
    in_code_block = False
    code_lang = None
    removed_blocks = 0

    while i < len(lines):
        line = lines[i]

        # Check for code block start
        if line.strip().startswith('```'):
            lang_match = re.match(r'^```(\w+)', line.strip())
            if lang_match:
                lang = lang_match.group(1).lower()
                if lang in ['python', 'java']:
                    # Skip Python/Java code blocks
                    in_code_block = True
                    code_lang = lang
                    removed_blocks += 1
                    i += 1
                    # Skip until closing ```
                    while i < len(lines) and not lines[i].strip().startswith('```'):
                        i += 1
                    # Skip the closing ```
                    if i < len(lines):
                        i += 1
                    continue
                elif lang == 'cpp':
                    # Keep C++ code blocks
                    in_code_block = True
                    code_lang = 'cpp'
                    new_lines.append(line)
                    i += 1
                    continue
            else:
                # Unknown code block, keep it
                new_lines.append(line)
                i += 1
                continue

        # Check for closing ```
        if in_code_block and line.strip().startswith('```'):
            in_code_block = False
            code_lang = None
            new_lines.append(line)
            i += 1
            continue

        # If we're in a Python/Java block, skip the line
        if in_code_block and code_lang in ['python', 'java']:
            i += 1
            continue

        # Otherwise, keep the line
        new_lines.append(line)
        i += 1

    print(f"Removed {removed_blocks} Python/Java code blocks")
    return new_lines

def main():
    filepath = '/root/cppinter/ALL_IN_ONE.md'
    print(f"Reading {filepath}...")

    lines = read_file(filepath)
    print(f"Original line count: {len(lines)}")

    # Step 1: Remove LCR section and 丑数 section
    print("\n=== Step 1: Removing LCR section and 丑数 section ===")
    lines = remove_lcr_and_丑数(lines)
    print(f"Line count after removing LCR and 丑数: {len(lines)}")

    # Step 2: Remove overlapping LeetCode problems
    print("\n=== Step 2: Removing overlapping LeetCode problems ===")
    lines = remove_overlapping_leetcode(lines)
    print(f"Line count after removing overlaps: {len(lines)}")

    # Step 3: Strip Python/Java code blocks
    print("\n=== Step 3: Stripping Python/Java code blocks ===")
    lines = strip_python_java(lines)
    print(f"Line count after stripping Python/Java: {len(lines)}")

    # Write the result
    output_path = '/root/cppinter/ALL_IN_ONE_MERGED.md'
    write_file(output_path, lines)
    print(f"\n=== Done! ===")
    print(f"Output written to: {output_path}")
    print(f"Final line count: {len(lines)}")
    print(f"Reduction: {59293 - len(lines)} lines ({100 * (59293 - len(lines)) / 59293:.1f}%)")

if __name__ == '__main__':
    main()
