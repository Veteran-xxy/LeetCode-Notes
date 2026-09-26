class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        int length = 0;
        unordered_set<int> seen(nums.begin(), nums.end());
        for(int x : seen){
            if(seen.count(x - 1) == 0){
                int cnt = 1;
                int current = x;
                while(seen.count(current + 1) > 0){
                    current++;
                    cnt++;
                }
                if(cnt > length){
                    length = cnt;
                }
            }
        }
        return length;
    }
};
