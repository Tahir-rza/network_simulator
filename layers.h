#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<chrono>
#include<map>
#include<vector>
#include <random>
#include <bitset>
#include <algorithm>
#include<iomanip>
#include <cstring>
using namespace std;

// Forward declaration of the Switch class
class Switch;

class EndDevices{
   private:
   int deviceId;
   string MAC_Address;
   string IP_Address;
   string message;
   public:
    map<string,string> arp;
    map<int,bool> selective_window;
    int sender_buffer;
    int reciever_buffer;
    bool ack;
    bool token;
    EndDevices() {
        deviceId = 0;
        MAC_Address="";
        ack=false;
        token=false;
    }
    EndDevices(int Id,string mac,string ip){
        deviceId=Id;
        MAC_Address=mac;
        IP_Address=ip;

    }
    int getId(){
        return deviceId;
    }
    string getMAC(){
        return MAC_Address;
    }
    string getIP(){
        return IP_Address;
    }
    void getData(string data){
        message=data;
        
       
    }
   string SendData(){
    return message;
   }
   //Application layer protocol
   // Callback function to write received data into a string
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append(static_cast<char*>(contents), totalSize);
        return totalSize;
}

   int http() {
   string domain;
    cout << "Enter domain name: ";
    cin >> domain;
    cout << std::endl;

    string command = "curl -s https://" + domain;

    // Open a pipe to read the response
    FILE* pipe = popen(command.c_str(), "r"); 
    if (!pipe) {
        cerr << "Error executing command." << std::endl;
        return 1;
    }

   
    string response;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        response += buffer;
    }

   
    pclose(pipe);

    
    std::cout << "Response:\n" << response << std::endl;

    return 0;
}


   void dns(){
      cout<<"DNS "<<endl;
      cout<<endl;
      string domain;
      cout<<"Enter domain name "<<endl;
      cin>>domain;

    
    string command = "nslookup " + domain;

    
    FILE* stream = popen(command.c_str(), "r");
    if (stream) {
       
        char buffer[256];
        while (!feof(stream) && fgets(buffer, sizeof(buffer), stream) != nullptr) {
            cout << buffer;
        }
        pclose(stream);
    } else {
        cout << "Failed to execute the command." << endl;
    }

   }
   void sendAck(int reciever){
     ack=true;
     cout<<endl;
     cout<<"Acknowledgement Status: "<<endl;
     cout<<endl;
     cout<<"Device "<<reciever<<" sends back ACK to Hub "<<endl;
     cout<<endl;
     
   }
   void tokenCheck(vector<EndDevices> &devices,int sender,int size){
      srand(time(nullptr));
      cout<<endl;
      cout<<"Token status :"<<endl;
      int i=rand()%size;
      //until sender doesn't have access to token
      while(devices[sender-1].token!=true){
        //logical ring
         int Current_device=(i%size);
         devices[Current_device].token=true;
         if(Current_device!=sender-1){
         cout<<"Currently sender doesn't have access to channel. Token is at device "<<Current_device+1<<" .Waiting to get access "<<endl;
         }
         i++;
         sleep(4);
      }
     
      cout<<"Sender has access to channel now"<<endl;
      
    }
    void sender(vector<int> window){
      srand(time(nullptr));
      cout<<endl;
       //window sliding 
       int i=0;
       while(i<window.size()){
           srand(time(0));
           ack=false;
           int timeout_duration=4,sending_time=rand()%6,recieving_time=rand()%6; 
           sender_buffer=window[i];
           //sending packet to reciever
           int AckNo;
           sleep(sending_time); 
           if(sending_time> timeout_duration){         //packet got lost
            cout<<"Sender sends packet with sequence number "<<window[i] <<" but it got lost"<<endl; 
            continue;          //resend packet
           }
           else{
            cout<<"Sender sends packet with sequence number "<<sender_buffer<<endl;
            AckNo=reciever(window,i);
            //reciever reaches at the end of window
              if(AckNo==-1){
                cout<<"Done"<<endl;
                break;
              }
            if(recieving_time>timeout_duration){      //ACK got lost
               cout<<"ACK "<<AckNo<< " got lost"<<endl;
               continue;           //resend packet
            }
            else{
              if(ack==true){
              cout<<"Sender recieves ACK "<<AckNo<<endl;  //ACK Recieved
              i++;
              }
            }
           } 
           
       }
       
    }
    int j=0;
    int reciever(vector<int> window,int i){
       
        
        if(sender_buffer==window[j] && i==j && j<window.size()){
        reciever_buffer=sender_buffer;
         ack=true;
         j++;
         if(j==window.size()){
          return -1;
         }
        return window[j];
         
        }
        else{
           cout<<"Packet "<<window[i]<<" was discarded as it a duplicate "<<endl;       //ACK Lost Case
           ack=true;
           return window[j];
        }

    }
    //Flow control Protocol
      void StopAndWait(){
        int windowSize=7;
        vector<int> window;
        for(int i=0;i<windowSize;i++){
          if(i%2==0){
            window.push_back(0);
          }
          else{
            window.push_back(1);
          }
        }
        cout<<endl;
        cout<<"Transmission Status :"<<endl;
        sender(window);
           
       }
      int R_n=0;
      int selective_reciever(int packet){
        
        selective_window[packet]=true;
        int AckNo=packet;
        //slide recieving window if consecutive elements are marked
        int count=0;
          for(int j=0;j<selective_window.size();j++){
                if(selective_window[j]==false){
                  break;
                }
                count++;
          }
          R_n=count;
          ack=true;
        return AckNo;

      }
    void selective_sender(){
           srand(time(nullptr));
           int S_n=0,S_f=1,S_z=selective_window.size();
           int i=0,AckNo;   
           while(i<S_z){
           srand(time(NULL));
           ack=false;
           int timeout_duration=4,sending_time=rand()%6,recieving_time=rand()%6; 
          
          
           sleep(sending_time); 
           if(sending_time> timeout_duration){         //packet got lost
            cout<<"Sender sends packet with sequence number "<<i <<" but it got lost"<<endl; 
            i++;
            continue;
           }
           else{
            int packet=i;
            cout<<"Sender sends packet with sequence number "<<packet<<endl;
            AckNo=selective_reciever(packet);
           
            if(recieving_time>timeout_duration){      //ACK got lost
               cout<<"ACK "<<AckNo<< " got lost"<<endl;
               i++;
            }
            else{
              if(ack==true){
                cout<<"ACK "<<AckNo<<" recieved"<<endl;
                int count=0;
                //slide window if consecutive elemnts in window are marked
              for(int j=0;j<=AckNo;j++){
                if(selective_window[j]==false){
                  break;
                }
                count++;
              }
              S_f=count;
              
              i++;
              S_n=i;
              }
            }
            
           }
           
          
         } 
         //timeout
          if(i==S_z){
            cout<<endl;
             cout<<"Time out occured"<<endl;
            
             //check which packet  is not recieved and resend it
             for(int j=0;j<selective_window.size();j++){
               
              if(selective_window[j]==false){
                cout<<"Resending Packet "<<j<<" as it wasn't recieved"<<endl;
                //resending packet
                AckNo=selective_reciever(j);
                cout<<"ACK "<<AckNo<<" recieved"<<endl;
              }
             }
           }
    }
   //Selective Repeat protocol
   void Selective_Repeat(){
    //m=4
    cout<<endl;
    int size=8;
    for(int i=0;i<size;i++){
      selective_window[i]=false;
    }
    selective_sender();

   }
    void prompt(string DeviceType,int d,map<int,bool> &mp){
     
     for(int i=1;i<=d;i++){
      mp[i]=true;
     }
     cout<<endl;
     cout<<"Choose the "<<DeviceType<< " device"<<endl;
     for(int i=0;i<mp.size();i++){
      cout<<i+1<<" : "<<"device "+to_string(i+1)<<endl;
     }
    }
   //arp cache
   void arp_cache(string ip,string mac){
        arp[ip]=mac;      
        
   }
   void print_ArpCache(){
        cout<<endl;
        cout<<"ARP Cache of sender is as :"<<endl;
        cout<<endl;
        cout<<"IP\t\t\tMAC\n";
        cout<<endl;
        for(auto it: arp){
          cout<<it.first<<"\t\t"<<it.second<<endl;
          cout<<endl;
        }
   }
};

class hub{
  private:
  int hub_ID;
          //vector for storing endDevice objects in hub
  public:
  vector<EndDevices> connected_devices;
  bool ack;
  string data;
  hub() {
        hub_ID= 0;
        ack=false;
    }
  hub(int Id){
        hub_ID=Id;
        
  }
  int getId(){
        return hub_ID;
   }
   //return devices connected to hub
  vector<EndDevices> getDevices(){
    return connected_devices;
  }
  void topology(EndDevices &devices){
    //connecting end devices to hub
    connected_devices.push_back(devices);
    
  }
  void connection(int i){
    for(int j=0;j<connected_devices.size();j++){
      cout<<"Connection Established between hub "<<i<<" and End device with ID "<<connected_devices[j].getId()<<endl;
    }
  } 
  void print_connection(int i){
    cout<<"Connection successfully created between hub and device "<<connected_devices[i].getId()<<endl;
  }
  
  void broadcast(vector<EndDevices> devices, int sender){
    cout<<endl;
    cout<<"A message is being broadcasted from the Hub"<<endl;
    cout<<endl;
    //hub is getting data from sender
    string data=devices[sender-1].SendData();
   
    //hub broadcasts data 
    for(int i=0;i<connected_devices.size();i++){
      // if(i!=sender-1){
      connected_devices[i].getData(data);
      // }
    }
    
  }
  
  //status of transmission
  void transmission(int sender,int reciever){
    cout<<endl;
    cout<<"Transmission status: "<<endl;
    cout<<endl;
    for(int i=0;i<connected_devices.size();i++){
       string message=connected_devices[i].SendData();
       int Current_device=connected_devices[i].getId();
       if(Current_device!=sender){
        if(Current_device!=reciever){
          cout<<message<<" was recieved by device "<<Current_device<<" but it was discarded"<<endl;
        }
        else{
          cout<<"Device "<<Current_device<<"' recieved message '"<<message<<"' successfully"<<endl;
        }
       }
    }
  }
  //broadcast Ack
  void BroadcastAck(int sender,int reciever){
     cout<<"Hub Broadcasts ACK"<<endl;
     cout<<endl;
     if(connected_devices[reciever-1].ack==true){
      for(int i=0;i<connected_devices.size();i++){
       int Current_device=connected_devices[i].getId();
       if(Current_device!=reciever){
       if(Current_device!=sender ){
         cout<<"ACK was recieved by device "<<Current_device<<" but it was discarded"<<endl;
       }
       else{
        cout<<"ACK was recieved by device "<<Current_device<<" and it was accepted"<<endl;
       }
      }
      }
     }
  }
};

class Process {
public:
    int processID;
    int portNumber;
    int assignPortNumber(std::map<int, Process>& processMap) {
    std::srand(std::time(nullptr));

    long long port =rand()%65535;
    while (processMap.find(port) != processMap.end() || port<1024) {
        port =rand()%65535;
    }

    return port;
}

};



class Router: public EndDevices{
 public:
 int id;
 int source;
 int destination;
 int weight;
 const int INF = 99999;
  
  string IP1,IP2,IP3,MAC1,MAC2,MAC3;
  vector<Switch> connected_devices;
  map<string,pair<string,string>> routing_table;
  //intialising ip and mac of interfaces of router
  Router(){}
  Router(int Id){
        id=Id;
  }
  int getId(){
    return id;
  }
  void setAddress(string IP1,string IP2,string IP3,string  MAC1,string  MAC2,string MAC3){
    this->IP1=IP1;
    this->IP2=IP2;
    this->MAC1=MAC1;
    this->MAC2=MAC2;
    this->IP3=MAC3;
  }
 
 
  //connect switch to router
  void ConnectSwitch(Switch& s){
    connected_devices.push_back(s);
  }
  //generate NID 
  
int random(int min, int max) {
 
  return rand() % (max - min + 1) + min;
}

// function to generate a random NID
    string generate_NID() {
      string NID = "";
      // Generate four octets randomly between 0 and 255
      for (int i = 0; i < 4; i++) {
        int octet = random(0, 255);
        // Append the octet to the IPv4 address with a dot
        if(i<2){
        NID += to_string(octet) + ".";
        }
        else{
          NID+=to_string(0)+".";
        }
      }
      // Remove the last dot
      NID.pop_back();
      return NID;
}
//generate classless IPV4 Address
string generate_classless_ip(string NID){
  NID.pop_back();
  
    for (int i = 0; i < 4; i++) {
        int octet = random(0, 255);
        // Append the octet to the IPv4 address with a dot
        
        if(i==3){
          NID+=to_string(octet)+"/24";
        }
      }
      
      return NID;
}
bool sameNID(string sourceIp,string destinationIp){
  for(int i=0;i<6;i++){
    if(sourceIp[i]!=destinationIp[i]){
      return false;
    }
  }
  return true;
}
int NetworkNo(string sourceIp){
  if(IP1.substr(0,6)==sourceIp.substr(0,6)){
    return 1;
  }
  else{
    return 2;
  }
}
void Routing_Table(Router &r,int source){
  //manually configure the router
  //NID  INTERFACE NEXT HOP
  routing_table[IP1]={"1","0"};
  routing_table[IP2]={"2","0"};
  if(source==1){
  routing_table[r.IP2]={"2",r.IP1};
  }
  else{
    routing_table[r.IP1]={"2",r.IP2};
  }
}
void Print_Routing_Table(int source) {
  cout<<endl;
  std::cout << std::endl;
  cout<<"Routing Table of Router "<<source<<endl;
  std::cout << std::left << std::setw(15) << "NID";
  std::cout << std::left << std::setw(12) << "Interface";
  std::cout << std::left << std::setw(10) << "Next Hop" << std::endl;

  std::cout << std::setfill('-') << std::setw(39) << "" << std::endl; // Separator line

  std::cout << std::setfill(' '); // Reset fill character

  for (auto it : routing_table) {
    std::cout << std::left << std::setw(15) << it.first;
    std::cout << std::left << std::setw(12) << it.second.first;
    std::cout << std::left << std::setw(10) << it.second.second << std::endl;
  }

  std::cout << std::endl;
}
//traverse through routing table and check for NID that matches destination ip
void routing_decision(string destinationIp){
  for(auto it:routing_table){
    //Check if NetworkId matches then break , it.first is NID HERE
    if(sameNID(it.first,destinationIp)){
       cout<<"Sending packet to Network "<<it.first<<" on interface "<<it.second.first<<endl;
       break;
    }
  }
}
void print_ArpCache(int source){
        cout<<endl;
        cout<<"ARP Cache of Router "<<source<<" is as :"<<endl;
        cout<<endl;
        cout<<"IP\t\tMAC\n";
        cout<<endl;
        for(auto it: arp){
          cout<<it.first<<"\t"<<it.second<<endl;
          cout<<endl;
        }
   }

 void RIP(const std::vector<std::vector<int>>& edges, int numVertices, int source) {
    vector<int> distance(numVertices, 1e9);
    distance[source] = 0;

    vector<int> nextHop(numVertices, -1); 

    
    for (int i = 1; i <= numVertices - 1; ++i) {
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];

            if (distance[u] != 1e9 && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                nextHop[v] = u; 
            }
        }
    }

    // Print the routing table
    cout<<endl;
    cout<<"Final Routing table"<<endl;
    std::cout << "Routing table for Router " << source << ":\n";
    std::cout << "Destination\tNext Hop\tCost\n";
    for (int i = 0; i < numVertices; ++i) {
        cout<<"R" <<i << "\t\t";
        if (distance[i] == 1e9) {
            cout << "-\t\t";
        } else if (i == source) {
            // std::cout <<"R"<< i << "\t\t" << distance[i] << "\n";
            cout << "-\t\t" << distance[i] << "\n";
        } else {
            if (nextHop[i] != -1 && source!=nextHop[i]) {
                cout <<"R"<< nextHop[i] << "\t\t";
                
            } else {
                cout << "-\t\t";
            }
            cout << distance[i] << "\n";
        }
    }
    cout << "\n";
}   
void initial_Routing_table(vector<vector<int>> &edges,int numVertices){
    
    std::cout << "Initial Routing Tables:\n";
    for (int source = 0; source < numVertices; ++source) {
        std::cout << "Routing table for Router " << source << ":\n";
        std::cout << "Destination\tNext Hop\tCost\n";
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "R" << i << "\t\t";
            if (i == source) {
                std::cout <<"-\t\t0\n";
            } else {
                bool directlyConnected = false;
                for (const auto& edge : edges) {
                    if (edge[0] == source && edge[1] == i) {
                        cout << "-\t\t" << edge[2] << "\n";
                        directlyConnected = true;
                        break;
                    }
                }
                if (!directlyConnected) {
                    std::cout << "-\t\t-\n";
                }
            }
        }
        std::cout << "\n";
    }
}


};

class Switch{
  private:
  int switchId;
  map<int, vector<int>> hub_DeviceMap;
  map<int,string> mac_table;
  
  vector<hub> connected_hubs;
  string data;
  public:
  vector<EndDevices> connected_devices;
   Switch(){
    data="";
   }
   Switch(string message){
     data=message;
   }
   void topology(EndDevices devices){
    //connecting end devices to switch
    connected_devices.push_back(devices);
   } 
  void print_connection(int i){
    cout<<"Connection successfully established between switch and device with MAC_Address: "<<connected_devices[i].getMAC()<<endl;
  }
  void topology(hub &hubs){
    //connecting hubs to switch
    connected_hubs.push_back(hubs);
  }
  void hub_print_connection(int i){
    cout<<"Connection successfully established between switch and hub with Hub ID : "<<connected_hubs[i].getId()<<endl;
  }
  vector<EndDevices> getDevices(){
    return connected_devices;
  }
  void HubToDeviceMap(int hubId,vector<EndDevices> &devices){
       vector<int> devices_id;
      for(int i=0;i<devices.size();i++){
        int id=devices[i].getId();
        devices_id.push_back(id);
      }
      //mapping between hub and end devices connected to it
      hub_DeviceMap[hubId]=devices_id;

  }
  void print_HubToDeviceMap(){
    cout<<"Mapping of Hub and End devices, stored in switch"<<endl;
    cout<<endl;
    for(auto it:hub_DeviceMap){
        int hubId = it.first+1;
        vector<int> deviceIds = it.second;
        cout << "End devices ";
        for (int i = 0; i < deviceIds.size(); i++) {
            cout << deviceIds[i];
            if (i < deviceIds.size() - 1) {
                cout << ",";
            }
        }
        cout << " are connected to hub " << hubId << endl;
        
         
    }
    cout<<endl;
  }
  int findHubForDevice(int deviceId) {
    for (auto iter = hub_DeviceMap.begin(); iter != hub_DeviceMap.end(); ++iter) {
        int hubId = iter->first;
        vector<int> deviceIds = iter->second;
        if (find(deviceIds.begin(), deviceIds.end(), deviceId) != deviceIds.end()) {
            return hubId;
        }
    }
    // Device not found in any hub
    return -1;
}

  void MAC_table(){
     
     //switch storing mapping of device id and MAC address
     for(int i=0;i<connected_devices.size();i++){
          int id=connected_devices[i].getId();
          string mac=connected_devices[i].getMAC();
          mac_table[id]=mac;
     }

  }
  int recieveData(int sender,int reciever,string message){
    // get data from sorce hub
    data=message;
    int source_hub=findHubForDevice(sender);
    int destination_hub=findHubForDevice(reciever);
    cout<<"Switch recieved "<<message<<" from hub "<<source_hub+1<<endl;
    //send data to destination hub
    connected_hubs[destination_hub].data=message;
    cout<<"Switch sends "<<message<<" to hub "<<destination_hub+1<<endl;
    return destination_hub;
  }
  void transmission(vector<EndDevices> &devices,int sender,int reciever){
    cout<<endl;
    cout<<"Transmission Status :"<<endl;
    cout<<endl;
    
    bool token=connected_devices[sender-1].token;
    string data=devices[sender-1].SendData();
      if(token==true){
      cout<<data<<" sent successfully from device with MAC "<<mac_table[sender]<< " to "<<mac_table[reciever]<<" via  switch"<<endl;
      }
  }
  //send ack to sender in case of end devices
  void sendAck(int sender){
   bool ack=connected_devices[sender-1].ack;
   if(ack==true){
   cout<<"ACK was successfully recieved by sender with MAC Address "<<mac_table[sender]<<endl;
   }
   else{
    cout<<"ACK not recieved by sender"<<endl;
   }
  
  } 
  //recieve ack from destination hub
  void recieveAck(int destination_hub){
   if(connected_hubs[destination_hub].ack==true){
    cout<<"Hub "<<destination_hub+1<<" sends ACK to switch"<<endl;
   }
  }
  //send ACK to source hub
  void send_Ack(int source_hub){
      connected_hubs[source_hub].ack=true;
      cout<<"Switch sends ACK to Hub "<<source_hub+1<<endl; 
  }

  //broadcast arp request
  string broadcast_Arp(string destinationIp,Router &r,int network){
    cout<<endl;
    cout<<"Switch broadcast ARP request"<<endl;
    cout<<endl;
    cout<<"Who is "<<destinationIp<<" ?"<<endl;
    cout<<endl;
     for(int i=0;i<connected_devices.size();i++){
      string result=connected_devices[i].arp[destinationIp];
      if(result.length()!=0){
         cout<<"ARP Reply : Source IP : "<<connected_devices[i].getIP()<<" Source MAC : "<<connected_devices[i].getMAC()<<endl;
       return connected_devices[i].getMAC();
      }
      
     }
     //return MAC address of default gateway
     if(network==1){
     cout<<"ARP Reply by Default Gateway : Source IP : "<<r.IP1<<" Source MAC : "<<r.MAC1<<endl;
     return r.MAC1;
     }
     else{
      cout<<"ARP Reply by Default Gateway : Source IP : "<<r.IP2<<" Source MAC : "<<r.MAC2<<endl;
     return r.MAC2;
     }
  
  
  }
   //send message
   void sendMessage(EndDevices & devices ,string destinationIP){
           cout<<endl;
           string destination_mac=devices.arp[destinationIP];
           string source_mac=devices.getMAC();
           cout<<devices.SendData()<<" sent successfully from device with MAC "<<source_mac<<" to "<<destination_mac<<endl;
   }
 
};


