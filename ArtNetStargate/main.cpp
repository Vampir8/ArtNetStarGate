/*BETA1.1_____          __                        __
        /  _  \________/  |_         ____   _____/  |_
       /  /_\  \_  __ \   __\  ___  /    \_/ __ \   __\
      /    |    \  | \/|  |   /__/ |   |  \  ___/|  |
      \____|__  /__|   |__|        |___|  /\___  >__|
  _________ __\/                ________\/     \/_
 /   _____//  |______ _______  /  _____/_____ _/  |_  ____
 \_____  \\   __\__  \\_  __ \/   \  ___\__  \\   __\/ __ \
 /        \|  |  / __ \|  | \/\    \_\  \/ __ \|  | \  ___/
/_______  /|__| (____  /__|    \______  (____  /__|  \___  >
        \/           \/               \/     \/          \/

*/

// "ws2_32"
#include <Windows.h>
#include <iostream>
#include <thread>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;
DWORD* adr;
char* progname ="XX";       //default program name
char* nodeip = "127.0.0.1"; //default node ip addres
//bind
int fps = 32;               //ms delay between arddmx frames
int universes = 2;          //nr of universes to send
int rauniverse = 8;         //nr of universe where Ra is (end of last universe is a best - defalult is 8 (8.508)




#ifdef _WIN32
#  include <WinSock2.h>
#  include <Ws2tcpip.h>
#  pragma comment(lib, "Ws2_32.lib")
#else
// Do something else here for non windows
#endif

void write(const std::string& sentence)
{
    using namespace std::chrono_literals; // do zapisu 100ms (potrzeba wsparcia C++14)

    for(auto letter : sentence)
    {
        std::cout << letter;
        std::cout.flush();
        std::this_thread::sleep_for( 30ms );
    }
}

char* GetAddressOfData(DWORD pid, const char *data, size_t len)
{
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if(process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        MEMORY_BASIC_INFORMATION info;
        std::vector<char> chunk;
        char* p = 0;
        while(p < si.lpMaximumApplicationAddress)

        {
            if(VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info))
            {
                p = (char*)info.BaseAddress;
                chunk.resize(info.RegionSize);
                SIZE_T bytesRead;
                if(ReadProcessMemory(process, p, &chunk[0], info.RegionSize, &bytesRead))
                {
                    for(size_t i = 0; i < (bytesRead - len); ++i)
                    {
                        if(memcmp(data, &chunk[i], len) == 0)
                        {
                            return (char*)p + i;
                        }
                    }
                }
                p += info.RegionSize;
            }
        }
    }

    return 0;
}

int searchRa()
{

    const char someData[] = {0x54, 0x68, 0x69, 0x65, 0x66};

    DWORD pid;
	HWND hand = FindWindow(NULL, progname);
	GetWindowThreadProcessId(hand, &pid);
    char* ret = GetAddressOfData(pid, someData, sizeof(someData));


    if(ret)
    {
        std::cout << ".*| I found RA |*."<< "\n";
        adr = (DWORD*)ret;
    }
    else
    {
        std::cout << "RA not found\n";

    }
    return 0;
}


//arg .exe nodeip fps universes Ra
int main(int argc, char **argv) {

if (argc >= 2){
    progname = argv[1];
}

if (argc >= 3){
    nodeip = argv[2];
}

//arg4 bind

if (argc >= 5){
    fps = atof(argv[4]);
}
if (argc >= 6){
    universes = atof(argv[5]);
    fps = fps/universes;
}
if (argc >= 7){
    rauniverse = atof(argv[6]);
}


    system("CLS");
    std::cout << R"(
BETA 1.1 _____          __                        __
        /  _  \________/  |_         ____   _____/  |_
       /  /_\  \_  __ \   __\  ___  /    \_/ __ \   __\
      /    |    \  | \/|  |   /__/ |   |  \  ___/|  |
      \____|__  /__|   |__|        |___|  /\___  >__|
  _________ __\/                ________\/     \/_
 /   _____//  |______ _______  /  _____/_____ _/  |_  ____
 \_____  \\   __\__  \\_  __ \/   \  ___\__  \\   __\/ __ \
 /        \|  |  / __ \|  | \/\    \_\  \/ __ \|  | \  ___/
/_______  /|__| (____  /__|    \______  (____  /__|  \___  >
        \/           \/               \/     \/          \/
)" << '\n';
    write("                      W3LCOM3 to StarGate              ");
    std::cout << "" << std::endl;
    if (!FindWindow(NULL, progname)){
            write("                 I can`t find ");
    write(progname);
    write(" program window.       ");

    std::cout << "" << std::endl;
    write("                 Press the any key to continue.");
    system("PAUSE >nul");
    }
    system("CLS");
    write("  Searching RA ...");
    std::cout << "" << std::endl;


    int iResult;
    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;
    sockaddr_in RecvAddr;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    int BufLen = 530;

    unsigned short Port = 6454;

            //---------------------------------------------
    // Create a socket for sending data
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "192.168.1.1")
    // and the specified port number.
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(Port);
    RecvAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket.
        if (argc >= 4 && 0 == strcmp(argv[3], "bind")){
    // Bind the socket.
    iResult = bind(SendSocket, (SOCKADDR *) &RecvAddr, sizeof (RecvAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
        return 1;
        }
    }

    RecvAddr.sin_addr.s_addr = inet_addr(nodeip);


    RecvAddr.sin_addr.s_addr = inet_addr(nodeip);

    char ArtPoolReply[239];
    ArtPoolReply[0] = byte('A'); // A
    ArtPoolReply[1] = byte('r'); // r
    ArtPoolReply[2] = byte('t'); // t
    ArtPoolReply[3] = byte('-'); // -
    ArtPoolReply[4] = byte('N'); // N
    ArtPoolReply[5] = byte('e'); // e
    ArtPoolReply[6] = byte('t'); // t
    ArtPoolReply[7] = 0x00;      // 0x00
    ArtPoolReply[8] = 0x00;     // OpCode[0]
    ArtPoolReply[9] = 0x21;     // OpCode[1]
    ArtPoolReply[10] = 127;     // IPV4 [0]
    ArtPoolReply[11] = 0;       // IPV4 [1]
    ArtPoolReply[12] = 0;       // IPV4 [2]
    ArtPoolReply[13] = 1;       // IPV4 [3]
    ArtPoolReply[14] = 0x36;    // IP Port Low
    ArtPoolReply[15] = 0x19;    // IP Port Hi
    ArtPoolReply[16] = 0x01;    // High byte of Version
    ArtPoolReply[17] = 0x02;    // Low byte of Version
    ArtPoolReply[18] = 0x00;    // NetSwitch
    ArtPoolReply[19] = 0x00;    // Net Sub Switch
    ArtPoolReply[20] = 0xFF;    // 0x04; // OEMHi
    ArtPoolReply[21] = 0xFF;    // 0xB9; // OEMLow
    ArtPoolReply[22] = 0x00;    // Ubea Version
    ArtPoolReply[23] = 0xF0;    // Status1
    ArtPoolReply[24] = 0x00;    // ESTA LO 0x41; //
    ArtPoolReply[25] = 0x00;    // ESTA HI  0x4D; //
    ArtPoolReply[26] = byte('S');  // S  //Short Name
    ArtPoolReply[27] = byte('t');  // t
    ArtPoolReply[28] = byte('a');  // a
    ArtPoolReply[29] = byte('r');  // r
    ArtPoolReply[30] = byte('G');  // G
    ArtPoolReply[31] = byte('a');  // a
    ArtPoolReply[32] = byte('t');  // t
    ArtPoolReply[33] = byte('e');  // e
    for (int i = 34; i <= 43; i++) {// Short Name
      ArtPoolReply[i] = 0x00;
    }
    ArtPoolReply[44] = byte('d');  // d  //Long Name
    ArtPoolReply[45] = byte('o');  // o
    ArtPoolReply[46] = byte('t');  // t
    ArtPoolReply[47] = byte('2');  // 2
    ArtPoolReply[48] = byte('S');  // S
    ArtPoolReply[49] = byte('t');  // t
    ArtPoolReply[50] = byte('a');  // a
    ArtPoolReply[51] = byte('r');  // r
    ArtPoolReply[52] = byte('G');  // g
    ArtPoolReply[53] = byte('a');  // a
    ArtPoolReply[54] = byte('t');  // t
    ArtPoolReply[55] = byte('e');  // e
    ArtPoolReply[56] = byte(' ');  //
    ArtPoolReply[57] = byte('v');  // v
    ArtPoolReply[58] = byte(' ');  //
    ArtPoolReply[59] = byte('1');  // 1
    ArtPoolReply[60] = byte('.');  // .
    ArtPoolReply[61] = byte('0');  // 0
    for (int i = 62; i <= 107; i++) { //Long Name
      ArtPoolReply[i] = 0x00;
    }
    for (int i = 108; i <= 171; i++) {  //NodeReport
      ArtPoolReply[i] = 0x00;
    }
    ArtPoolReply[172] = 0x00; // NumPorts Hi
    ArtPoolReply[173] = 0x08; // NumPorts Lo
    ArtPoolReply[174] = 0xC0; // Port 0 Type
    ArtPoolReply[175] = 0xC0; // Port 1 Type
    ArtPoolReply[176] = 0xC0; // Port 2 Type
    ArtPoolReply[177] = 0xC0; // Port 3 Type
    ArtPoolReply[178] = 0x80; // GoodInput 0
    ArtPoolReply[179] = 0x80; // GoodInput 1
    ArtPoolReply[180] = 0x80; // GoodInput 2
    ArtPoolReply[181] = 0x80; // GoodInput 3
    ArtPoolReply[182] = 0x80; // GoodOutput 0
    ArtPoolReply[183] = 0x80; // GoodOutput 1
    ArtPoolReply[184] = 0x80; // GoodOutput 2
    ArtPoolReply[185] = 0x80; // GoodOutput 3
    ArtPoolReply[186] = 0x00; // SwIn 0
    ArtPoolReply[187] = 0x01; // SwIn 1
    ArtPoolReply[188] = 0x02; // SwIn 2
    ArtPoolReply[189] = 0x03; // SwIn 3
    ArtPoolReply[190] = 0x00; // SwOut 0//ODBIERA UNIVERSE NR
    ArtPoolReply[191] = 0x01; // SwOut 1
    ArtPoolReply[192] = 0x02; // SwOut 2
    ArtPoolReply[193] = 0x03; // SwOut 3
    ArtPoolReply[194] = 0x01; // SwVideo
    ArtPoolReply[195] = 0x00; // SwMacro
    ArtPoolReply[196] = 0x00; // SwRemote
    ArtPoolReply[197] = 0x00; // Spare
    ArtPoolReply[198] = 0x00; // Spare
    ArtPoolReply[199] = 0x00; // Spare
    ArtPoolReply[200] = 0x00; // Style
    // MAC ADDRESS
    ArtPoolReply[201] = 0x00; // MAC HI
    ArtPoolReply[202] = 0x00; // MAC
    ArtPoolReply[203] = 0x00; // MAC
    ArtPoolReply[204] = 0x00; // MAC
    ArtPoolReply[205] = 0x00; // MAC
    ArtPoolReply[206] = 0x00; // MAC LO
    ArtPoolReply[207] = 0x00; // BIND IP 0
    ArtPoolReply[208] = 0x00; // BIND IP 1
    ArtPoolReply[209] = 0x00; // BIND IP 2
    ArtPoolReply[210] = 0x00; // BIND IP 3
    ArtPoolReply[211] = 0x00; // BInd Index




    char ArtDmx[530];
    //make artdmx buffor
    ArtDmx[0] = 0x41; //ID
    ArtDmx[1] = 0x72;
    ArtDmx[2] = 0x74;
    ArtDmx[3] = 0x2d;
    ArtDmx[4] = 0x4e;
    ArtDmx[5] = 0x65;
    ArtDmx[6] = 0x74;
    ArtDmx[7] = 0x00;
    ArtDmx[8] = 0x00; //OpCode
    ArtDmx[9] = 0x50;
    ArtDmx[10] = 0x00;  //ProtoVer
    ArtDmx[11] = 0x0e;
    ArtDmx[12] = 0x00;  //Sequence 0-ff
    ArtDmx[13] = 0x00;  //Physical
    ArtDmx[14] = 0x00;  //Universe
    ArtDmx[15] = 0x00;
    ArtDmx[16] = 0x02;  //Length
    ArtDmx[17] = 0x00;
    for (int i = 18; i <=530; i++){
        ArtDmx[i] = 0x00;
    }



    if (FindWindow(NULL, progname)){
    DWORD PID;
	HWND hand = FindWindow(NULL, progname);
	GetWindowThreadProcessId(hand, &PID);
	HANDLE process = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, PID);
	char value[512];

	//ReadProcessMemory(process, (LPVOID)adr, value, 6, NULL);
    searchRa();

    if (universes > 16) { universes = 16;}

    std::cout << "StarGate Activated" << std::endl;
    std::cout << "  IP:" <<nodeip << std::endl;

    for (int uni = 0; uni < universes; uni++){
           std::cout << "" << std::endl;
           std::cout << ".*| Universe ";
           std::cout << uni;
           std::cout << " |*.";


    }

    int Sequence = 0;
    while (FindWindow(NULL, progname)){   //get universe data and send ArtDMX
        if (Sequence == 0 && nodeip == "127.0.0.1"){
            iResult = sendto(SendSocket, ArtPoolReply, BufLen, 0, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));
            if (iResult == SOCKET_ERROR) {
                wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
                closesocket(SendSocket);
                WSACleanup();
                return 1;
                }
            }
        ArtDmx[12] = Sequence * 0x01;
        Sequence ++;

        int adrminus = rauniverse * 512 - 5;


        for (int u = 0; u < universes; u++){

        ReadProcessMemory(process, (LPVOID)adr-adrminus, value, 512, NULL);   // universe 0

        for (int i = 0; i < 512; i++) {
            ArtDmx[14] = u - 0x00;
        ArtDmx[18+i] = value[i];
            }
        adrminus -= 512;

    //---------------------------------------------
    // Send a datagram to the receiver
    iResult = sendto(SendSocket, ArtDmx, BufLen, 0, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
        closesocket(SendSocket);
        WSACleanup();
        return 1;
        }
    Sleep(fps);
    }

    if (Sequence == 255){
            Sequence = 0;
    }
    //Sleep(fps);
    //adrminus = 4091;
    }


    }
    system("CLS");
    std::cout << "Program is off" << std::endl;
    //---------------------------------------------
    // When the application is finished sending, close the socket.
    wprintf(L"Finished sending. Closing socket.\n");
    iResult = closesocket(SendSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Clean up and quit.
    wprintf(L"Exiting.\n");
    WSACleanup();

    //system("PAUSE >nul");
    return(0);
}



