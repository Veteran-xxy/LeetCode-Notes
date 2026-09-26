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
