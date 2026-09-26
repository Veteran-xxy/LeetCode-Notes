# 49. 字母异位词分组 Group Anagrams

- 难度：Medium
- 主要知识点：字符串、排序、哈希表
- 使用语言：C++
- 日期：2026-09-26
- 状态：已在 LeetCode 验收通过

## 一、题目理解

给定一个字符串数组，要把由相同字母组成、只是排列顺序不同的单词放进同一组，返回所有分组。组与组的顺序没有要求。空字符串也可以作为输入。

## 二、动手前补的 C++ 基础

我学过 Java、Python、C，但还没学过 C++。起初直接看到“排序、分组”，不知道该从哪里写起。后来先从题目给的函数签名学起：

- `vector<string>& strs` 是字符串数组的引用；`strs[i]` 是第 `i` 个字符串，`strs.size()` 是数组长度。
- `vector<vector<string>>` 是“很多组字符串”，对应 Python 中的二维列表。
- `string` 可以用下标访问字符，例如 `strs[1][0]` 是下标为 1 的字符串中下标为 0 的字符。下标从 0 开始，我最初把 `strs[1]` 当成第一个单词，练习后才分清。
- `""` 是长度为 0 的空字符串，`" "` 是长度为 1、包含空格的字符串。我最初把它们混在一起了。
- `push_back()` 把元素加到 `vector` 末尾；`unordered_map<string, vector<string>>` 表示“字符串键 → 一组字符串”。第一题的哈希表保存的是“数字 → 下标”，这次值变成了一个 `vector<string>`。

这些只是为了读懂和写出代码所补的语法。下面的排序分组思路是 Codex 引导我理解的，不是我一开始自己想到的。

## 三、为什么排序后可以分组

把一个单词的字母排序后，可以得到用于分组的键：

```text
"eat" → "aet"
"tea" → "aet"
"tan" → "ant"
```

前两个单词得到同一个键，就应该进入同一组。`sort(key.begin(), key.end())` 会直接修改 `key`，所以先用 `string key = strs[i];` 复制原单词，再排序副本。放进组里的仍是原单词 `strs[i]`，不能只保存排序后的字符串。

## 四、我第一次尝试时卡住的地方

第一次写代码时，我用了 `str[i]`，但参数名其实是 `strs`；还写了类似 `sortedkey[i] = sort(key.begin(), key.end());`。这有两个问题：`sortedkey` 没有声明，而且 `sort()` 不返回排序后的字符串，它直接修改传入范围内的内容。

我还想再写一个循环，用 `if` 逐个比较排序结果，并尝试对 `groups` 调用 `push_back()`。这是把容器的职责弄混了：`groups` 是哈希表，不是 `vector`；真正能 `push_back()` 的是 `groups[key]` 对应的那一组。相同的键由哈希表自动找到同一个位置，不需要再用第二层循环比较所有单词。

改成一次遍历后，我又把 `groups[key].push_back(strs[i]);` 中的 `strs` 误写成了 `str`。这只是变量名笔误，但 C++ 找不到未声明的 `str`。

## 五、从哈希表取出结果时的错误

分组完成后，我写过 `result = entry.second`，末尾还漏了分号。`entry.second` 是一组字符串，类型为 `vector<string>`；`result` 是所有组，类型为 `vector<vector<string>>`。不能用一组直接给所有组赋值，应使用 `result.push_back(entry.second);` 把这一组追加进去。

遍历哈希表时，`entry.first` 是排序后的键，`entry.second` 是这个键对应的一组原单词。由于 `unordered_map` 没有固定遍历顺序，返回的各组顺序也不固定，符合题意。

## 六、最终通过的代码

最终代码也保存在 [solution.cpp](./solution.cpp)：

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;
        for(int i = 0; i < strs.size(); i++){
            string key = strs[i];
            sort(key.begin(), key.end());
            groups[key].push_back(strs[i]);
        }
        vector<vector<string>> result;
        for(auto& entry : groups){
            result.push_back(entry.second);
        }
        return result;
    }
};
```

## 七、复杂度

设 `n` 是字符串个数，`k` 是最长字符串的长度。每个字符串复制并排序，排序最多花 `O(k log k)`，因此平均时间复杂度为 `O(n · k log k)`；哈希表操作按平均情况分析，键的哈希还要读取字符。哈希表保存原单词，结果数组再收集各组，整体额外空间复杂度为 `O(n · k)`。

## 八、这题学到的 C++

- `string` 的下标、`size()`、空字符串，以及复制后修改副本。
- `sort(key.begin(), key.end())` 原地排序，返回值不能当作排好序的字符串。
- `unordered_map<string, vector<string>>` 的键和值可以分别是字符串和字符串数组；`groups[key]` 在键不存在时会创建一个空组。
- `groups[key].push_back(strs[i])` 是往某个键对应的组里添加原单词。
- `for (auto& entry : groups)` 遍历键值对；`entry.second` 是值，`result.push_back(entry.second)` 才是把一组加入最终结果。

这次最重要的收获是把“排序生成分组键”和“保存原单词”分开想，再让哈希表按键完成分组。写代码时也要先分清每个变量的类型：哈希表、单独的一组、所有组，各自能做的操作不同。
