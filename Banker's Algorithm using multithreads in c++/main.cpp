#include <iostream>
#include <thread>
#include <vector>
using namespace std;

bool allcompleted = false, current = true, total = true;
int rows, cols, process_to_perform;

void isvalid(vector<vector<int>> needed, vector<int> available, vector<bool> status)
{
    total = false;
    bool temp = true;
    for (int j = 0; j < rows; j++)
    {
        if (total)
        {
            return;
        }
        if (status[j] == true)
        {
            continue;
        }
        for (int i = 0; i < cols; i++)
        {
            if (available[i] < needed[j][i])
            {
                temp = false;
            }
        }

        if (temp)
        {
            total = true;
        }
    }
}

void current_process(vector<int> process, vector<int> available, int index, vector<bool> status)
{
    if (status[index])
    {
        return;
    }
    for (int i = 0; i < cols; i++)
    {
        if (process[i] > available[i])
        {
            return;
        }
    }
    process_to_perform = index;
    current = true;
}

void isdone(vector<bool> status)
{
    for (auto x : status)
    {
        if (x == false)
        {
            allcompleted = false;
            return;
        }
    }
    allcompleted = true;
}

int main()
{
    cout << "Enter no of processes - ";
    cin >> rows;
    cout << "Enter no of resources for each process - ";
    cin >> cols;

    vector<vector<int>> allotted(cols, vector<int>(rows, 0));
    vector<vector<int>> needed(cols, vector<int>(rows, 0));
    vector<vector<int>> maximum(cols, vector<int>(rows, 0));
    vector<int> available(rows);
    vector<bool> status(cols, false);
    cout << "\nEnter Allotted resources for each process" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cin >> allotted[i][j];
        }
    }
    cout << "\nEnter Maximum requried resources for each process" << endl;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cin >> maximum[i][j];
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            needed[i][j] = maximum[i][j] - allotted[i][j];
        }
    }
    cout << "\nEnter Available resources " << endl;

    for (int i = 0; i < cols; i++)
    {
        cin >> available[i];
    }

    thread validate(isvalid, needed, available, status);
    validate.join();

    cout << "\n\nThe process starts\n"
         << endl;

    while (total && !allcompleted)
    {

        current = false;
        thread eachprocess[3];
        for (int i = 0; i < rows; i++)
        {
            eachprocess[i] = thread(current_process, needed[i], available, i, status);
        }

        for (int i = 0; i < rows; i++)
        {
            eachprocess[i].join();
        }

        for (int i = 0; i < cols; i++)
        {
            available[i] += allotted[process_to_perform][i];
            allotted[process_to_perform][i] = 0;
        }
        status[process_to_perform] = true;
        cout << "Process " << process_to_perform + 1 << " Completed" << endl;

        thread alldone(isdone, status);
        alldone.join();
        thread validate(isvalid, needed, available, status);
        validate.join();
    }

    if (!allcompleted)
    {
        cout << "Deadlock occurred the process cannot continue any further ";
    }
    else
    {
        cout << "All the process are done with execution";
    }

    return 0;
}