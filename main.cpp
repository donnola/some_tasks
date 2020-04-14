#include <iostream>
#include <vector>

using namespace std;

class Image {
public:
    Image(size_t main_dem_x, size_t main_dem_y, size_t cut_dem_x, size_t cut_dem_y);
    pair<size_t, size_t> Find_coordinates();
    string Find_direction();
private:
    void PrefixFunction (const vector<size_t>& pattern, vector<size_t>& buf_pat);
    vector<vector<size_t>> main_image;
    vector<vector<size_t>> cut_image;
};

Image::Image(size_t main_dem_x, size_t main_dem_y, size_t cut_dem_x, size_t cut_dem_y) {
    cout << "введите главное изображение по строкам сверху вниз" << endl;
    for (size_t i = 0; i < main_dem_y; ++i) {
        vector<size_t> temp;
        for(size_t j = 0; j < main_dem_x; ++j) {
            size_t num;
            cin >> num;
            temp.push_back(num);
        }
        main_image.push_back(temp);
    }
    cout << "введите вырезку из главного изображения по строкам сверху вниз" << endl;
    for (size_t i = 0; i < cut_dem_y; ++i) {
        vector<size_t> temp;
        for(size_t j = 0; j < cut_dem_x; ++j) {
            size_t num;
            cin >> num;
            temp.push_back(num);
        }
        cut_image.push_back(temp);
    }
}

void Image::PrefixFunction (const vector<size_t>& pattern, vector<size_t>& buf_pat) {
    for (int i = 1; i < pattern.size(); ++i) {
        int len = buf_pat[i-1];
        while ((len > 0) && (pattern[i] != pattern[len])) {
            len = buf_pat[len - 1];
        }
        if (pattern[len] == pattern[i]) {
            ++len;
        }
        buf_pat[i] = len;
    }
}

pair<size_t, size_t> Image::Find_coordinates() {
    bool first_find = false;
    vector<vector<size_t>> pref(main_image.size(), vector<size_t> (main_image[0].size()));
    size_t row_in_cat_image = 0;
    pair<size_t, size_t> last_inter(0, 0);
    size_t next_row = 0;
    while (row_in_cat_image < cut_image.size()) {
        size_t cur_len = 0;
        size_t str_ind = 0;
        vector<size_t> buf_pat(cut_image[0].size());
        PrefixFunction(cut_image[row_in_cat_image], buf_pat);
        for (size_t j = last_inter.first; j < main_image[0].size(); ++j) {
            if (str_ind == 0 && cut_image[row_in_cat_image][0] == main_image[next_row][j]) {
                cur_len = 1;
                if (cur_len == cut_image[0].size()) {
                    if (!first_find) {
                        first_find = true;
                        last_inter = pair(j, next_row);
                        ++next_row;
                    }
                    ++row_in_cat_image;
                    break;
                }
            }
            if (str_ind > 0) {
                while ((cur_len > 0) && (main_image[next_row][j] != cut_image[row_in_cat_image][cur_len])) {
                    cur_len = buf_pat[cur_len - 1];
                }
                if (cut_image[row_in_cat_image][cur_len] == main_image[next_row][j]) {
                    ++cur_len;
                }
                if (cur_len == cut_image[0].size()) {
                    if (!first_find) {
                        first_find = true;
                        last_inter = pair(str_ind + 1 - cur_len, next_row);
                        ++next_row;
                        ++row_in_cat_image;
                        break;
                    }
                    if (first_find && last_inter.first == str_ind + 1 - cur_len) {
                        ++row_in_cat_image;
                        break;
                    }
                    if (first_find && last_inter.first != str_ind + 1 - cur_len){
                        first_find = false;
                        if (last_inter.first == main_image[0].size() - cut_image[0].size()) {
                            next_row = last_inter.second + 1;
                            last_inter.first = 0;
                            ++last_inter.second;
                        }
                        else {
                            next_row = last_inter.second;
                            ++last_inter.first;
                        }
                        row_in_cat_image = 0;
                        break;
                    }
                }
                else if (j == main_image[0].size() - 1) {
                    ++last_inter.second;
                    next_row == last_inter.second;
                    last_inter.first = 0;
                    row_in_cat_image = 0;
                    break;
                }
            }
            ++str_ind;
        }
    }
    return last_inter;
}

string Image::Find_direction() {
    string res;
    return res;
}

int main() {
    size_t main_dem_x;
    size_t main_dem_y;
    size_t cut_dem_x;
    size_t cut_dem_y;
    size_t var;
    cout << "введите размеры главного изборажения, сначала по оси x, затем по оси y" << endl;
    cin >> main_dem_x >> main_dem_y;
    cout << "введите размеры вырезки из главного изборажения, сначала по оси x, затем по оси y" << endl;
    cin >> cut_dem_x >> cut_dem_y;
    cout << "если хотите полчуить точные координаты введите 1, если ориентир - 2" << endl;
    cin >> var;
    Image image(main_dem_x, main_dem_y, cut_dem_x, cut_dem_y);
    pair<size_t, size_t> res_coordinates;
    string res_direction;
    if (var == 1) {
        res_coordinates = image.Find_coordinates();
        cout << res_coordinates.first << " " << res_coordinates.second;
    }
    else {
        res_direction = image.Find_direction();
    }
    return 0;
}
