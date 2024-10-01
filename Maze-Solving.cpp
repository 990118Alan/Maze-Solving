#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include<iostream>
#include<stack>
using namespace std;
using namespace cv;
vector<pair<int, int>> Move;
bool Find = false;
void dfs(int row, int col, Mat matrix) {
	matrix.at<uchar>(row, col) = 2;
	if ((row == matrix.rows - 1 || col == matrix.cols - 1 || row == 0 || col == 0) && !(row == 0 && col == 1)) {
		Find = true;
	}

	for (int i = 0; i < 4; i++) {
		int next_row = row;
		int next_col = col;
		next_row += Move[i].first;
		next_col += Move[i].second;
		if (next_row >= 0 && next_row < matrix.rows && next_col >= 0 && next_col < matrix.cols) {
			if (matrix.at<uchar>(next_row, next_col) == 0) {
				dfs(next_row, next_col, matrix);
				if (Find) { return; }//若找到出口了，不用走完剩下方向
			}
		}
	}
	if (!Find) {
		matrix.at<uchar>(row, col) = 0;
		return;
	}
}
int main(int argc, char* argv[]) {
	//--------------------------
	//圖片轉數字
	Mat maze = imread(argv[1], 1);
	Mat small_maze;
	resize(maze, small_maze, Size(maze.rows / 10, maze.cols / 10));
	Mat result(small_maze.size(), CV_8UC1);
	maze.release();
	for (int y = 0; y < small_maze.rows; y++) {
		for (int x = 0; x < small_maze.cols; x++) {
			Vec3b pixel = small_maze.at< Vec3b>(y, x);
			if (pixel == Vec3b(0, 0, 0)) {
				result.at<uchar>(y, x) = 1;  //黑色->1
			}
			else if (pixel == Vec3b(255, 255, 255)) {
				result.at<uchar>(y, x) = 0;  // 白色->0
			}
		}
	}

	// -----------------------
	//處理圖片
	//移動順序:下右上左
	Move.push_back(make_pair(1, 0));
	Move.push_back(make_pair(0, 1));
	Move.push_back(make_pair(-1, 0));
	Move.push_back(make_pair(0, -1));
	//進行dfs，尋找迷宮出口
	dfs(0, 1, result);
	//將迷宮內數值轉顏色，再存成圖片輸出
	for (int y = 0; y < small_maze.rows; y++) {
		for (int x = 0; x < small_maze.cols; x++) {
			uchar pixel = result.at< uchar>(y, x);
			if (pixel == 1) {
				small_maze.at<Vec3b>(y, x) = Vec3b(0, 0, 0);  //1->黑色
			}
			else if (pixel == 0) {
				small_maze.at<Vec3b>(y, x) = Vec3b(255, 255, 255);  //0-> 白色
			}
			else if (pixel == 2) {
				small_maze.at<Vec3b>(y, x) = Vec3b(0, 255, 0);//2->綠色
			}
		}
	}
	resize(small_maze, small_maze, Size(small_maze.rows * 10, small_maze.cols * 10), 0, 0, cv::INTER_NEAREST);
	imwrite(argv[2], small_maze);

	small_maze.release();
	result.release();
}