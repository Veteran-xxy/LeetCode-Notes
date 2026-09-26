# 1. 两数之和 Two Sum

- 难度：Easy
- 主要知识点：数组、哈希表
- 使用语言：C++
- 日期：2026-09-26

## 一、题目理解

给定整数数组 `nums` 和目标值 `target`，要找到两个**不同位置**的元素，使 `nums[i] + nums[j] == target`，并返回它们的**下标** `i` 和 `j`，而不是元素值。题目保证有且只有一个答案，同一个元素不能用两次。

## 二、我的第一思路：暴力枚举

我最先想到的是用两层循环：`i` 选第一个数字，`j` 选第二个数字，逐对判断它们的和是否等于 `target`。

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }
};
```

`j` 从 `i + 1` 开始，一是避免拿同一个元素和自己相加，二是避免检查过 `(0, 1)` 后又检查 `(1, 0)`。

## 三、我第一次写代码时犯的错误

### 1. 数组越界

我一开始把循环条件写成 `i <= nums.size()` 和 `j <= nums.size()`。如果数组有 4 个元素，`nums.size() == 4`，合法下标却只有 `0、1、2、3`；访问 `nums[4]` 就越界了。循环条件应写成 `i < nums.size()`、`j < nums.size()`。

记住：`size()` 是元素**数量**，非空数组的最大下标是 `size() - 1`。

### 2. 返回了元素值，而不是下标

我还写过 `return {nums[i], nums[j]};`。例如 `nums = [2, 7, 11, 15]`、`target = 9`，这会返回 `{2, 7}`；题目要的是 `{0, 1}`。正确写法是 `return {i, j};`：`nums[i]` 是元素值，`i` 才是下标。

## 四、为什么有 `return {}`

函数的返回类型是 `vector<int>`，所以最后也要有对应的返回语句。`return {};` 可以理解为 `return vector<int>{};`，即返回空 `vector`，有点像 Python 的 `return []`。本题保证有答案，正常情况下不会走到这里；保留它让函数的返回路径完整。

## 五、暴力解法复杂度

时间复杂度是 `O(n²)`：每个位置都可能与后面多个位置比较。额外空间复杂度是 `O(1)`：只用了少量变量。

## 六、我是怎么想到优化方向的

暴力解法是在问：“当前这个数应该和后面的哪个数相加？”我把问题换了一种问法：当前的 `nums[i]` 已经确定，另一个数只能是 `target - nums[i]`。

```cpp
int need = target - nums[i];
```

接下来只需问：“`need` 以前出现过吗？”如果能快速回答，就不用再用第二层循环逐个尝试。这是我从 `O(n²)` 想到哈希表解法的关键一步。

## 七、为什么使用 `unordered_map`

```cpp
unordered_map<int, int> record;
```

我用它保存“数字 → 下标”。例如 `2 → 0`、`7 → 1`、`11 → 2`：键是数组里的数字，值是这个数字的下标。`record[nums[i]] = i;` 把当前数字及其位置记下来，供后面的数字查找。

## 八、我一开始对 `unordered_map` 的错误理解

第一次尝试时，我仍想写第二层循环：

```cpp
for (int j = 0; j < record.size(); j++) {
    if (need == record[j]) {
        // ...
    }
}
```

这里我把 `unordered_map` 当成了 `vector`。`vector` 可以按连续的下标 `0、1、2……` 访问；`unordered_map` 存的是“键 → 值”，键可能是 `2`、`100`、`-7`，并不连续。`record[j]` 表示查找**键为 j** 的值，不是取“第 j 个元素”；键不存在时它还会插入一个默认值。真正需要的是直接查找 `need` 这个键。

## 九、`find()` 和 `end()`

`record.find(need)` 查找键 `need`，找到时返回指向该键值对的迭代器；没找到时返回 `record.end()`。所以：

```cpp
if (record.find(need) != record.end()) {
    return {record[need], i};
}
```

条件成立表示 `need` 已出现过。`record[need]` 取得它对应的值，也就是之前那个数字的下标。

## 十、为什么一定要先查，再存

用 `nums = [3, 3]`、`target = 6` 检查顺序。如果 `i = 0` 时先存 `3 → 0`，再查 `need = 3`，就会错误地返回 `{0, 0}`，等于把同一个位置用了两次。

正确顺序是：先算 `need`，再查它是否**此前**出现过，若没找到才存当前数字。第一次查不到 3，于是存 `3 → 0`；第二次查到先前的 3，返回 `{0, 1}`。这样 `record` 里始终只保存“之前见过的元素”。

## 十一、最终哈希表解法

最终代码也保存在 [solution.cpp](./solution.cpp)：

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> record;

        for (int i = 0; i < nums.size(); i++) {
            int need = target - nums[i];

            if (record.find(need) != record.end()) {
                return {record[need], i};
            }

            record[nums[i]] = i;
        }

        return {};
    }
};
```

## 十二、最终复杂度分析

时间复杂度**平均**为 `O(n)`：数组只遍历一遍，`unordered_map` 的查找和插入平均是 `O(1)`。这里不能说哈希表操作永远是 `O(1)`。额外空间复杂度是 `O(n)`：最多需要存入接近 `n` 个数字及其下标。

## 十三、本题学到的 C++ 知识

### `vector`

`vector<int>` 是动态数组。`nums[i]` 访问下标 `i` 的元素，`nums.size()` 获取元素数量。

### 引用 `&`

`vector<int>& nums` 中的 `&` 表示引用。函数可以直接使用传入的 `vector`，避免复制整个数组。目前先记住它的这个作用。

### `unordered_map`

`unordered_map<int, int>` 可先理解成 Python 的 `dict` 或 Java 的 `HashMap`，存放键值对：

- `record[key] = value;`：插入或修改。
- `record[key]`：取得键对应的值。
- `record.find(key) != record.end()`：判断键是否存在。
- `record.count(key)`：也可判断键是否存在，结果为 0 或 1。
- `record.erase(key)`：删除这个键。
- `record.size()`：获取键值对数量。

有个容易踩的坑：如果 `key` 不存在，`record[key]` 会插入这个键，并为这里的 `int` 值生成默认值 `0`。只想判断键是否存在时，用 `find()` 或 `count()` 更合适。

## 十四、本题最重要的收获

我最该记住的不是一段哈希表模板，而是**换个问法**：已知当前数字 `nums[i]`，我需要的 `target - nums[i]` 有没有在之前出现？记录见过的数字，就能省掉反复比较后面元素的过程。这是一次“用空间换时间”的练习。

这道题也让我实际用到了 `vector`、引用 `&`、`unordered_map`、`find()`、`end()`、`return {}`，以及时间和空间复杂度分析。下次写循环时，我会先分清“元素数量”和“最大下标”，返回答案时也要再看清题目要的是值还是下标。
