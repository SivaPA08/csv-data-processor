
#include "include/Evaluator.hpp"
// #include "include/SaveCSV.hpp"
#include "include/TablePrinter.hpp"
#include "src/doth/builddata.h"
#include "src/doth/splitbypipe.h"
#include "src/doth/splitbyspace.h"
#include "src/doth/tokenoperation.h"
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp> // json parser
#include <nlohmann/json_fwd.hpp>
#include <omp.h>
#include <sys/socket.h>
using namespace std;
using json = nlohmann::json;

BuildData buildData;
ifstream head("file.csv");
ifstream file("file.csv");
vector<string> columnNames = buildData.getColumnNames(head);
vector<vector<double>> csvdata = buildData.getData(file);

json siva(string cmd) {
    SplitByPipe splitbypipe;
    TokenOperation tokenOperation;
    SplitBySpace splitbyspace;
    Evaluator eval;

    // BuildData buildData;
    // ifstream head("file.csv");
    // ifstream file("file.csv");
    // cout << "Loading data..." << endl;
    // vector<string> columnNames = buildData.getColumnNames(head);
    // vector<vector<double>> data = buildData.getData(file);

    // for (int i = 0; i < (int)columnNames.size(); i++) {
    //     cout << columnNames[i] << " ";
    // }
    // cout << endl;

    cout << "Welcome to csv-data-processor" << endl;
    int columnLength = (int)columnNames.size();
    int rowLength = (int)csvdata.size();

    bool executeFinalOutput = false;
    cout << "csv-data-processor>>>";
    vector<string> a;
    string s = cmd;
    // if (s.empty())
    //     continue;
    vector<string> command = splitbypipe.split(s);

    // if (command[0] == "exit") {
    //     break;
    // // }
    // if (command[0] == "commit") {
    //     CSVWriter csvWriter;
    //     csvWriter.save(columnNames, data);
    //     break;
    // }

    double totalTimer = 0.0;
    // #pragma omp parallel for schedule(dynamic) if (command.size() > 1000)
    for (size_t cmd_idx = 0; cmd_idx < command.size(); cmd_idx++) {
        string str = command[cmd_idx];
        double startTimer = omp_get_wtime();

        if (!str.empty() && str[0] == 'a') {
            executeFinalOutput = true;

            string insideParanthesis = tokenOperation.getInsideParanthesis(str);
            vector<string> token =
                tokenOperation.seperateByCommas(insideParanthesis);

            int index = -1;
            for (int i = 0; i < (int)columnNames.size(); i++) {
                if (columnNames[i] == token[0]) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                // cout << "Column not found" << endl;
                a.push_back("Error");
            } else {
                int start = (token[1] == "*") ? 0 : stoi(token[1]);
                int end = (token[2] == "*") ? rowLength - 1 : stoi(token[2]);
                if (start < 0)
                    start = 0;
                if (end >= rowLength)
                    end = rowLength - 1;

                double share = 0.0;

#pragma omp parallel for reduction(+ : share) schedule(static)
                for (int row = start; row <= end; row++) {
                    share += csvdata[row][index];
                }

                a.push_back(to_string(share));
            }
        }

        else if (!str.empty() && str[0] == 'u') {
            executeFinalOutput = true;

            string insideParanthesis = tokenOperation.getInsideParanthesis(str);
            vector<string> token =
                tokenOperation.seperateByCommas(insideParanthesis);

            int index = -1;
            for (int i = 0; i < (int)columnNames.size(); i++) {
                if (columnNames[i] == token[0]) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                // cout << "Column not found" << endl;
                a.push_back("Error");
            } else {
                int start = (token[1] == "*") ? 0 : stoi(token[1]);
                int end = (token[2] == "*") ? rowLength - 1 : stoi(token[2]);
                if (start < 0)
                    start = 0;
                if (end >= rowLength)
                    end = rowLength - 1;

#pragma omp parallel for schedule(static)
                for (int row = start; row <= end; row++) {
                    double share = csvdata[row][index];
                    csvdata[row][index] = eval.calculate("x", share, token[3]);
                }

                a.push_back("Null");
            }
        }

        else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'a') {
            executeFinalOutput = true;

            string insideParanthesis = tokenOperation.getInsideParanthesis(str);
            vector<string> token =
                tokenOperation.seperateByCommas(insideParanthesis);

            int index = -1;
            for (int i = 0; i < (int)columnNames.size(); i++) {
                if (columnNames[i] == token[0]) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                // cout << "Column not found" << endl;
                a.push_back("Error");
            } else {
                string condition = token[1];
                double share = 0.0;

#pragma omp parallel for reduction(+ : share) schedule(dynamic)
                for (int row = 0; row < rowLength; row++) {
                    if (eval.evaluate("x", csvdata[row][index], condition)) {
                        share += csvdata[row][index];
                    }
                }

                a.push_back(to_string(share));
            }
        }

        else if (str.size() >= 2 && str[0] == 'c' && str[1] == 'u') {
            executeFinalOutput = true;

            string insideParanthesis = tokenOperation.getInsideParanthesis(str);
            vector<string> token =
                tokenOperation.seperateByCommas(insideParanthesis);

            int index = -1;
            for (int i = 0; i < (int)columnNames.size(); i++) {
                if (columnNames[i] == token[0]) {
                    index = i;
                    break;
                }
            }

            if (index == -1) {
                // cout << "Column not found" << endl;
                a.push_back("Error");
            } else {
                string condition = token[2];

#pragma omp parallel for schedule(static)
                for (int row = 0; row < rowLength; row++) {
                    if (eval.evaluate("x", csvdata[row][index], condition)) {
                        double share = csvdata[row][index];
                        csvdata[row][index] =
                            eval.calculate("x", share, token[2]);
                    }
                }

                a.push_back("Null");
            }
        }

        else if (!str.empty() && str[0] == 's') {
            string insideParanthesis = tokenOperation.getInsideParanthesis(str);
            vector<string> token =
                tokenOperation.seperateByCommas(insideParanthesis);
            vector<string> inputedColNames = splitbyspace.split(token[0]);
            vector<int> indexes(inputedColNames.size(), -1);

            for (int i = 0; i < (int)inputedColNames.size(); i++) {
                for (int j = 0; j < (int)columnNames.size(); j++) {
                    if (columnNames[j] == inputedColNames[i]) {
                        indexes[i] = j;
                        break;
                    }
                }
            }

            bool ok = true;
            for (int i : indexes) {
                if (i == -1) {
                    ok = false;
                    break;
                }
            }

            if (!ok) {
                // cout << "Column not found" << endl;
                a.push_back("Error");
            } else {
                vector<vector<string>> table;
                vector<string> header;
                for (int idx : indexes) {
                    header.push_back(columnNames[idx]);
                }
                table.push_back(header);
                table.resize(rowLength + 1);

#pragma omp parallel
                {
#pragma omp for schedule(dynamic) nowait
                    for (int i = 0; i < rowLength; i++) {
                        vector<string> row;
                        row.reserve(indexes.size());
                        for (int idx : indexes) {
                            row.push_back(to_string(csvdata[i][idx]));
                        }
                        table[i + 1] = std::move(row);
                    }

#pragma omp barrier
#pragma omp single
                    {
                        TablePrinter::print(table, 12);
                    }
                }
            }
        }

        else {
#pragma omp critical
            {
                cout << "Unknown command: " << str << endl;
            }
        }

        double endTimer = omp_get_wtime();
        totalTimer += (endTimer - startTimer);
    }

    // cout << "Execution Time: " << totalTimer << endl;
    vector<vector<string>> finalOutput;
    finalOutput.push_back(command);
    finalOutput.push_back(a);
    // TablePrinter::print(finalOutput, 20);
    // cout << endl;

    json response;
    response["status"] = "ok";
    response["command"] = command;
    response["result"] = a;
    response["execution_time"] = totalTimer;

    return response;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        return 1;
    }

    cout << "Server listening on port 9000\n";
    while (true) {

        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            perror("accept");
            return 1;
        }

        // recived command
        uint32_t len;
        if (recv(clientSocket, &len, sizeof(len), MSG_WAITALL) <= 0) {
            close(clientSocket);
            close(serverSocket);
            return 0;
        }

        len = ntohl(len);

        string cmd(len, '\0');
        if (recv(clientSocket, cmd.data(), len, MSG_WAITALL) <= 0) {
            close(clientSocket);
            close(serverSocket);
            return 0;
        }

        // process command
        json response = siva(cmd);

        // send json res
        string payload = response.dump();
        uint32_t out_len = htonl(payload.size());

        send(clientSocket, &out_len, sizeof(out_len), 0);
        send(clientSocket, payload.data(), payload.size(), 0);

        close(clientSocket);
    }
    close(serverSocket);
    return 0;
}
