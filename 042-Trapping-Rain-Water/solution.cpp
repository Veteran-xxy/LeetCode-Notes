class Solution {
public:
    int trap(vector<int>& height) {
        int area = 0;
        int maxindex = 0;
        int maxHeight = 0;

        for (int i = 0; i < height.size(); i++) {
            if (height[i] >= maxHeight) {
                maxHeight = height[i];
                maxindex = i;
            }
        }

        for (int i = 0; i < maxindex; i++) {
            if (height[i] > height[i + 1]) {
                area += height[i] - height[i + 1];
                height[i + 1] = height[i];
            }
        }

        for (int i = height.size() - 1; i > maxindex; i--) {
            if (height[i] > height[i - 1]) {
                area += height[i] - height[i - 1];
                height[i - 1] = height[i];
            }
        }

        return area;
    }
};
