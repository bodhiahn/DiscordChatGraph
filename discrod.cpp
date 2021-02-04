//g++ -std=c++11 discrod.cpp -o discrod -Iinclude -Llib -lxd -lglad -lglfw3 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
// ^ to build

#include <fstream>
#include <iostream>
#include <string>
#include <xd/xd.hpp>

using namespace std;

using namespace xd;
const string target = "game";
const string username1 = "sqwigly";
const string username2 = "julz";
const int XVAL = 800;
const int YVAL = 800;
vector<int> user1;
vector<int> user2;
vector<int> date;
double weight = 1.0;
int firstYear = 2018;

void setup() {
    vector<string> fulldate;
    vector<int> fulluser1;
    vector<int> fulluser2;
    int num1 = 0;
    int num2 = 0;
    bool check = false;
    size(XVAL, YVAL);
    string line;
    ifstream file("julz.txt");
    bool tempUser = false;
    string halfDate;
    //generate values
    while (getline(file, line)) {
        if (check) {
            if (line.find(target) != string::npos) {
                if (tempUser)
                    num1++;
                else
                    num2++;
            }
            fulluser1.push_back(num1);
            fulluser2.push_back(num2);
            if (halfDate == "18" || halfDate == "19" || halfDate == "20" || halfDate == "21") {
                fulldate.push_back("20" + halfDate);
            }
            check = false;
        } else if (line.front() == '[' && (line.find(username1) || line.find(username2))) {
            if (line.find(username1) != string::npos) {
                tempUser = true;
            } else {
                tempUser = false;
            }
            if (line.length() > 10) {
                halfDate = line.substr(8, 2);
            }
            check = true;
        }
    }
    file.close();

    // make date set
    string curyear = fulldate.at(0);
    firstYear = stoi(fulldate.at(0));
    date.push_back(50);
    for (int i = 0; i < fulldate.size(); i++) {
        if (curyear != fulldate.at(i)) {
            double pos = ((XVAL - 50) * ((i * 1.0) / (fulldate.size() * 1.0))) - 20;
            date.push_back((int)pos);
            curyear = fulldate.at(i);
        }
    }

    // average it to size of screen
    if (fulluser1.size() > (XVAL - 100)) {
        double diff = fulluser1.size() / (XVAL - 100);
        for (int i = 0; i < XVAL - 100; i++) {
            user1.push_back(fulluser1.at(i * diff));
        }
    } else {
        for (int i = 0; i < fulluser1.size(); i++) {
            user1.push_back(fulluser1.at(i));
        }
        for (int i = fulluser1.size(); i < XVAL - 100; i++) {
            user1.push_back(fulluser1.at(fulluser1.size() - 1));
        }
    }

    // squash or stretch data to size of screen horizontally
    if (fulluser2.size() > (XVAL - 100)) {              // if larger than space on screen
        double diff = fulluser2.size() / (XVAL - 100);  // find the percent difference in size
        for (int i = 0; i < XVAL - 100; i++) {
            user2.push_back(fulluser2.at(i * diff));  // and only add that percent
        }
    } else {
        for (int i = 0; i < fulluser2.size(); i++) {
            user2.push_back(fulluser2.at(i));
        }
        for (int i = fulluser2.size(); i < XVAL - 100; i++) {
            user2.push_back(fulluser2.at(fulluser2.size() - 1));
        }
    }

    // squash or stretch data to size of screen vertically
    int last1 = user1.at(user1.size() - 1);
    int last2 = user2.at(user2.size() - 1);

    if (last1 > last2 && ((last1 > YVAL - 100 || (last1 < ((YVAL - 100) / 2))))) {  // check for the set with the largest final value
        weight = ((YVAL - 100) * .75) / last1;                                      // and if its not within the margin of the screen, set a weight to make the data end up 3/4 of the way up
    } else if (last2 > YVAL - 100 || (last2 < ((YVAL - 100) / 2))) {
        weight = ((YVAL - 100) * .75) / last2;
    }
}

void draw() {
    stroke(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    background(vec4(0.1f, 0.1f, 0.1f, 1.0f));
    rect(50, 50, 700, 700);
    for (int i = 0; i < date.size(); i++) {
        text(to_string(firstYear + i), date.at(i), YVAL - 35);
    }

    text(target, XVAL / 2, 25);
    text("0", 35, YVAL - 52);
    stroke(vec4(1.0f, 0.0f, 0.0f, 1.0f));
    text(username1, 55, 70);
    text(to_string(user1.at(user1.size() - 1)), 0, YVAL - 52 - (user1.at(user1.size() - 1) * weight));
    for (int i = 0; i < user1.size() - 1; i++) {
        line(i + 50, YVAL - 52 - (user1.at(i) * weight), i + 51, YVAL - 52 - (user1.at(i + 1) * weight));
    }
    stroke(vec4(0.0f, 1.0f, 0.0f, 1.0f));
    text(username2, 55, 90);
    text(to_string(user2.at(user2.size() - 1)), 0, YVAL - 52 - (user2.at(user2.size() - 1) * weight));
    for (int i = 0; i < user2.size() - 1; i++) {
        line(i + 50, YVAL - 52 - (user2.at(i) * weight), i + 51, YVAL - 52 - (user2.at(i + 1) * weight));
    }
}

void destroy() {
}