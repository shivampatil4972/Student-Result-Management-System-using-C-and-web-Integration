#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Database.h"
#include "include/json.hpp"

using namespace std;
using json = nlohmann::json;

class WebServer {
private:
    Database& db;
    bool isRunning;
    SOCKET listenSocket;
    HANDLE hThread;
    DWORD threadId;

    string readFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return "";
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void handleRequest(SOCKET clientSocket) {
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            closesocket(clientSocket);
            return;
        }
        buffer[bytesReceived] = '\0';
        string request(buffer);

        string response;
        if (request.find("GET /api/students") == 0) {
            vector<Student> students = db.getAllStudents();
            json j = json::array();
            for (const auto& s : students) j.push_back(s.toJson());
            
            string body = j.dump();
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
        } 
        else if (request.find("GET /styles.css") == 0) {
            string body = readFile("styles.css");
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
        }
        else if (request.find("GET /script.js") == 0) {
            string body = readFile("script.js");
            response = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
        }
        else if (request.find("GET / ") == 0 || request.find("GET /student_report.html") == 0) {
            string body = readFile("student_report.html");
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
        }
        else {
            string body = "Not Found";
            response = "HTTP/1.1 404 Not Found\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
        }

        send(clientSocket, response.c_str(), response.length(), 0);
        closesocket(clientSocket);
    }

    struct ClientData {
        WebServer* server;
        SOCKET clientSocket;
    };

    static DWORD WINAPI RequestThreadFunc(LPVOID lpParam) {
        ClientData* data = (ClientData*)lpParam;
        data->server->handleRequest(data->clientSocket);
        delete data;
        return 0;
    }

    static DWORD WINAPI ServerThreadFunc(LPVOID lpParam) {
        WebServer* server = (WebServer*)lpParam;
        server->runServerLoop();
        return 0;
    }

    void runServerLoop() {
        while (isRunning) {
            SOCKET clientSocket = accept(listenSocket, NULL, NULL);
            if (!isRunning) break;
            if (clientSocket != INVALID_SOCKET) {
                ClientData* data = new ClientData{this, clientSocket};
                HANDLE hClientThread = CreateThread(NULL, 0, RequestThreadFunc, data, 0, NULL);
                if (hClientThread) {
                    CloseHandle(hClientThread); // Detach the thread to run independently
                } else {
                    handleRequest(clientSocket); // Fallback to synchronous if thread fails
                    delete data;
                }
            }
        }
    }

public:
    WebServer(Database& database) : db(database), isRunning(false), listenSocket(INVALID_SOCKET), hThread(NULL) {}

    void start() {
        if (isRunning) {
            cout << "\n\t\t\t Server is already running!" << endl;
            return;
        }
        
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(8080);

        bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
        listen(listenSocket, SOMAXCONN);

        isRunning = true;
        
        cout << "\n\t\t\t Starting Live Web Server on http://localhost:8080..." << endl;
        cout << "\t\t\t The server is now running in the background!" << endl;
        
        system("start http://localhost:8080");

        // Spawn a background thread for the server loop so the console menu isn't blocked!
        hThread = CreateThread(NULL, 0, ServerThreadFunc, this, 0, &threadId);
        if (hThread == NULL) {
            cout << "\n\t\t\t Failed to create server background thread!" << endl;
            isRunning = false;
        }
    }

    void stop() {
        if (!isRunning) {
            cout << "\n\t\t\t Server is not running." << endl;
            return;
        }
        isRunning = false;
        
        // Closing the listen socket will break the accept() blocking call in the thread
        closesocket(listenSocket);
        
        if (hThread) {
            WaitForSingleObject(hThread, 1000);
            CloseHandle(hThread);
            hThread = NULL;
        }
        
        WSACleanup();
        cout << "\n\t\t\t Server stopped successfully." << endl;
    }
};
