class Solution {
public:
    int maxArea(vector<int>& height) {
        int max = 0;
        int left = 0;
        int area = 0;
        int right = height.size() - 1;
        while(left < right){
            if(height[left] < height[right]){
                area = height[left] * (right - left);
                left++;
            }
            else{
                area = height[right] * (right - left);
                right--;
            }
            if(max < area){
                max = area;
            }
        }
        return max;
    }
};
