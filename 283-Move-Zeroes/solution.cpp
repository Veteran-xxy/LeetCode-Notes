class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int loc = 0;
        for(int i = 0; i < nums.size(); i++){
            if(nums[i] != 0){
                int mid = nums[i];
                nums[i] = 0;
                nums[loc] = mid;
                loc++;
            }
        }
    }
};
