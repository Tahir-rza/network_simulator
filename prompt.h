#include<iostream>
#include "layers.h"
#include<time.h>
#include<map>
#include<vector>
using namespace std;

class physical_prompt{
  public:
    void run(){
    while(true){
    int d,sender,reciever;
    string data;
    map<int,bool> mp;
    hub h;
    EndDevices end;
    //vector of objects of End devices
    
    vector<EndDevices> devices;
    
     cout<<"Enter  the number of end devices"<<endl;
    cin>>d;
    if(d<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
    }
    
    for(int i=0;i<d;i++){
      //creating end devices
      devices.push_back(EndDevices(i+1,"",""));
      //connecting end devices with hub
      h.topology(devices[i]);
      if(i==0){
        cout<<"Connection status :"<<endl;
        cout<<endl;
      }
      h.print_connection(i);
    }
    
   
    end.prompt("sender",d,mp);
    cin>>sender;
    if(!mp[sender]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    end.prompt("reciever",d,mp);
    cin>>reciever;
    if(!mp[reciever]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    //if sender and reciever are same
    if(sender==reciever){
      cout<<"Sender and reciever can't be same "<<endl;
      continue;
    }
    cout<<endl;
    cout<<"Input the message that you would like to send "<<endl;
    cin.ignore();
    getline(cin,data);
  
   
    //pass data to sender
    devices[sender-1].getData(data);
    //broadcasting data
    h.broadcast(devices,sender);
    //transmission status
    cout<<endl;
    h.transmission(sender,reciever);
     //reciever sends back ack to hub
     cout<<endl;
    devices[sender-1].sendAck(reciever);
    //hub broadcast Ack
    h.BroadcastAck(sender,reciever);
    
    break;
  }
 }
};

class data_prompt{
  public:
  
  string generateMacAddress() {
    
    char mac[18];
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    return mac;
  }
  void run(int choice,int hubSize){
    while(true){
    int size,sender,reciever;
    string data;
    vector<EndDevices> devices; 
    map<int,bool> mp;
    hub h;
    EndDevices end;
   
    Switch s;
    cout<<endl;
    int select;
    if(choice==1){
    
     cout<<endl;
    //vector of objects of End devices
  
    cout<<"Enter  the number of end devices"<<endl;
    cin>>size;
    if(size<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
    }
    
    map<int,string> flow;
    flow={
      {1,"Stop and Wait ARQ"},
      {2,"Selective Repeat"}
    };
    cout<<endl;
    cout<<"Choose a Flow Control Protocol :"<<endl;
    cout<<endl;
    for(auto it:flow){
      cout<<it.first<<" : "<<it.second<<endl; 
    }

    cin>>select;
   
    if(select!=1 && select!=2){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    srand(time(0));
    for(int i=0;i<size;i++){
      //creating end devices
      string mac=generateMacAddress();
      devices.push_back(EndDevices(i+1,mac,""));
      //connecting devices with switch
      s.topology(devices[i]);
      if(i==0){
        cout<<"Connection status :"<<endl;
        cout<<endl;
      }
      s.print_connection(i);
    }
       
    end.prompt("sender",size,mp);
    cin>>sender;
    if(!mp[sender]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    end.prompt("reciever",size,mp);
    cin>>reciever;
    if(!mp[reciever]){
      cout<<"Invalid Entry"<<endl;
      continue;
    }
    //if sender and reciever are same
    if(sender==reciever){
      cout<<"Sender and reciever can't be same "<<endl;
      continue;
    }
    cout<<endl;
    cout<<"Input the message that you would like to send "<<endl;
    cin.ignore();
    getline(cin,data);
    //pass data to sender
    devices[sender-1].getData(data);
   
    //token passing
    cout<<endl;
    devices[sender-1].tokenCheck(devices,sender,size);
    //switch stores mapping of device id and MAC address
    s.MAC_table();
    cout<<endl;
    //for Stop and wait ARQ
    if(select==1){
      
       devices[sender-1].StopAndWait();
       cout<<endl;
       s.transmission(devices,sender,reciever);
       break;

    }
    //for Selective repeat
    else if(select==2){
      devices[sender-1].Selective_Repeat();
      cout<<endl;
      s.transmission(devices,sender,reciever);
      break;
    }
    
   }
    else if(choice==2){
      vector<EndDevices> devices2;
      Switch s2;
      hub h2;
     
      int deviceNum;
      string data2;
      vector<hub> hub_vec;
      map<int,bool> mp2;
      cout<<endl;
     
      
     
       for(int i=0;i<hubSize;i++){
        //creating objects of hub
         hub_vec.push_back(hub(i+1));
         //connecting hub with switch
       
        s2.topology(hub_vec[i]);
        if(i==0){
          cout<<"Connection status :"<<endl;
          cout<<endl;
        }
        s2.hub_print_connection(i);
       }
       cout<<endl;
       cout<<"Enter the number of end devices to be connected to each hub"<<endl;
       cin>>deviceNum;
      if(deviceNum<2){
        cout<<"There should be atleast two devices. Enter valid number"<<endl;
        continue;
      }
      int id=1,k=0;
      for(int i=0;i<hub_vec.size();i++){
        for(int j=0;j<deviceNum;j++){
          devices2.push_back(EndDevices(id,"",""));
          //connecting end devices with hub
          hub_vec[i].topology(devices2[k++]);
          id++;
      }
      }
      for(int i=0;i<hub_vec.size();i++){
        cout<<endl;
        hub_vec[i].connection(i+1);

      }
      cout<<endl;
      for(int i=0;i<hub_vec.size();i++){
        //getting array of connected end devices to hub[i]
        vector<EndDevices> connected_devices=hub_vec[i].getDevices();
        //mapping of hub and devices connected to it
        s2.HubToDeviceMap(i,connected_devices);
      }
      s2.print_HubToDeviceMap();
      int total_devices=deviceNum*hubSize;
      end.prompt("sender",total_devices,mp2);
      cin>>sender;

      if(!mp2[sender]){
        cout<<"Invalid Entry"<<endl;
        continue;
      }
      end.prompt("reciever",total_devices,mp2);
      cin>>reciever;
      if(!mp2[reciever]){
        cout<<"Invalid Entry"<<endl;
        continue;
      }
      //if sender and reciever are same
      if(sender==reciever){
        cout<<"Sender and reciever can't be same "<<endl;
        continue;
      }
      cout<<endl;
      cout<<"Input the message that you would like to send "<<endl;
      cin.ignore();
      getline(cin, data2);
      int source_hub=s2.findHubForDevice(sender);
      //pass data to sender
      devices2[sender-1].getData(data2);
      //hub recieves data
      hub_vec[source_hub].data=data2;
      //source hub broadcasts data
      hub_vec[source_hub].broadcast(devices2,sender);
      // //transmission status
      cout<<endl;
      hub_vec[source_hub].transmission(sender,reciever);
      //source hub sends data to switch
      int destination_hub=s2.recieveData(sender,reciever,data2);
      //destination hub broadcasts data
      hub_vec[destination_hub].broadcast(devices2,sender);
      // //transmission status
      cout<<endl;
      hub_vec[destination_hub].transmission(sender,reciever);
      //reciever sends back ack to hub
      cout<<endl;
      devices2[sender-1].sendAck(reciever);
      //destination hub broadcast Ack
      hub_vec[destination_hub].BroadcastAck(sender,reciever);
      //destination sends ack to switch
      hub_vec[destination_hub].ack=true;
      s2.recieveAck(destination_hub);
      //switch sends ack to source hub
      s2.send_Ack(source_hub);
      //source hub broadcast ACK
      hub_vec[source_hub].BroadcastAck(sender,reciever);
      break;
   }
   }
  }
};

class network_prompt{
  public:
  Switch s1,s2;
  EndDevices end;
  vector<EndDevices> devices;
  vector<Process> processes;
  map<int,Process> processMap;
  map<int,bool> mp;
  string message;
  string ip;
  int scheme;
  data_prompt d1;
  Router rt;
  string create_NID(){
          return rt.generate_NID();
  }
  void run(){
    while(true){
     cout<<endl;
     cout<<"Select routing scheme "<<endl;
     cout<<"1. Static Routing"<<endl;
     cout<<"2. Dynamic Routing"<<endl;
     cout<<endl;
     cin>>scheme;
     cout<<endl;
     //static Routing
     if(scheme==1){
      Router r1,r2,r;
      string nid1=create_NID();
      string nid2=create_NID();
      string nid3=create_NID();
      string nid4=create_NID();
      r1.setAddress(nid1,nid2,"",d1.generateMacAddress(),d1.generateMacAddress(),"");
      r2.setAddress(nid3,nid4,"",d1.generateMacAddress(),d1.generateMacAddress(),"");

      vector<string> ipv4;
        for(int i=0;i<4;i++){
        if(i<2){
          ip=r.generate_classless_ip(nid1);
          ipv4.push_back(ip);

        }
        else{
          ip=r.generate_classless_ip(nid4);
          ipv4.push_back(ip);
        }
      }
      //  //end devices in Network 1
      devices.push_back(EndDevices(1,d1.generateMacAddress(),ipv4[0]));
      devices.push_back(EndDevices(2,d1.generateMacAddress(),ipv4[1]));
      
        //end devices in Network 2
      devices.push_back(EndDevices(4,d1.generateMacAddress(),ipv4[2]));
      devices.push_back(EndDevices(5,d1.generateMacAddress(),ipv4[3]));
       //  //connecting switches to router
      r1.ConnectSwitch(s1);
      r2.ConnectSwitch(s2);
      //create processes on source machine
      for(int i=0;i<4;i++){
        Process p;
        processes.push_back(p);

      }
      for(int i=0;i<4;i++){
        
        processMap[processes[i].assignPortNumber(processMap)] = processes[i];
      }
      int sender, reciever;
      end.prompt("Sender",4,mp);
      cin>>sender;
      end.prompt("Reciever",4,mp);
      cin>>reciever;
      //if sender and reciever are same
        if(sender==reciever){
        cout<<"Sender and reciever can't be same "<<endl;
        continue;
        }
        cout<<endl;
        int App_Protocol;
        cout<<"Choose a protocol"<<endl;
        cout<<endl;
        cout<<"1. Http"<<endl;
        cout<<"2. DNS"<<endl;
        cin>>App_Protocol;
        cout<<endl;
        cout<<"Enter a message "<<endl;
        cin>>message;
        cout<<endl;
        
        devices[sender-1].getData(message);
        string SourceIp=devices[sender-1].getIP();
        string DestinationIp=devices[reciever-1].getIP();
        cout<<endl;
        cout<<endl;
        int sourcePort,destinationPort;
        int random=rand()%4;
        sourcePort=processes[random].assignPortNumber(processMap);
        destinationPort=(App_Protocol==1)?80:53;
        cout<<"Source IP : "<<SourceIp<<endl;
        cout<<"Source Port : "<<sourcePort<<endl;
        cout<<"Destination IP : "<<DestinationIp<<endl;
        cout<<"Destination Port : "<<destinationPort<<endl;
        sleep(4);
        cout<<endl;
        //   //iniatialise arp cache
        for(int i=0;i<4;i++){
          string deviceIp=devices[i].getIP();
          string deviceMac=devices[i].getMAC();
          devices[i].arp_cache(deviceIp,deviceMac);
        }
        
        //    //connecting end devices to respective switches
        s1.connected_devices.push_back(devices[0]);
        s1.connected_devices.push_back(devices[1]);
        s2.connected_devices.push_back(devices[2]);
        s2.connected_devices.push_back(devices[3]);
           devices[sender-1].print_ArpCache();

           
       // if sender and reciever are in same network
       bool check=r.sameNID(SourceIp,DestinationIp);
      
          int network=r1.NetworkNo(SourceIp);
       if(check){
        
        //Both sender and reciever are in same network
        //sender will check in its arp cache for entry of reciever
        string isPresent=devices[sender-1].arp[DestinationIp];
        if(!isPresent.length()){
          //send arp request
          cout<<endl;
          cout<<"Sender sends ARP request"<<endl;
          
          if(network==1){
            
            string destination_Mac=s1.broadcast_Arp(DestinationIp,r,network);
            //sender updates its arp cache
            devices[sender-1].arp_cache(DestinationIp,destination_Mac);
            
            cout<<"Updated Arp cache :"<<endl;
            devices[sender-1].print_ArpCache();
            //send message
            cout<<endl;
              sleep(4);
              cout<<endl;
             cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;
              s1.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }

              

          }
          else if(network==2){
            string destination_Mac=s2.broadcast_Arp(DestinationIp,r,network);
            //sender updates its arp cache
            devices[sender-1].arp_cache(DestinationIp,destination_Mac);
            cout<<"Updated Arp cache :"<<endl;
            devices[sender-1].print_ArpCache();
            //send message
            cout<<endl;
            sleep(4);
           cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;
              s2.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }
          }
        }
        else{
          //destination ip is in arp cache of sender no need to send arp request
          if(network==1){
            cout<<endl;
           cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;
              s1.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }
        }
          else if(network==2){
             cout<<endl;
           cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;
              s2.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }
          }
      
       }

       }
      else{
        
          //sender and reciever are in two different networks
          //sender checks for destination ip in its arp cache
          string result=devices[sender-1].arp[DestinationIp];
          
          //reciever is not in arp cache of sender
          if(result.length()==0){
            //sender sends arp request
            cout<<endl;
            cout<<"Sender sends ARP request"<<endl;
            if(network==1){
              //switch broadcast arp request
            
              string destination_Mac=s1.broadcast_Arp(DestinationIp,r1,network);
              // sender updates its arp cache
              devices[sender-1].arp_cache(r1.IP1,destination_Mac);
              cout<<"Updated ARP cache :"<<endl;
              devices[sender-1].print_ArpCache();
              //print routing table 
              r1.Routing_Table(r2,1);
              cout<<endl;
              r1.Print_Routing_Table(network);
               //check ARP CACHE of router
              r1.arp_cache(r1.IP1,r1.MAC1);
              r1.arp_cache(r1.IP2,r1.MAC2);
              r1.arp_cache(r2.IP1,r2.MAC1);
              cout<<endl;
              r1.print_ArpCache(network);
              cout<<endl;
              //traverse through routing table and check for NID that matches destination ip
              r1.routing_decision(DestinationIp);
             
              r2.Routing_Table(r1,2);
              cout<<endl;
              r2.Print_Routing_Table(2);
               //check ARP CACHE of router
              r2.arp_cache(r2.IP1,r2.MAC1);
              r2.arp_cache(r2.IP2,r2.MAC2);
              r2.arp_cache(r1.IP2,r1.MAC2);
              cout<<endl;
              r2.print_ArpCache(2);
              cout<<endl;
              cout<<"Router 2 sends ARP request "<<endl;
              cout<<endl;
              //switch will broadcast arp request 
              destination_Mac=s2.broadcast_Arp(DestinationIp,r,1);
              //sender updates its arp cache
              devices[sender-1].arp_cache(DestinationIp,destination_Mac);
              cout<<endl;
              sleep(4);
           cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;
              s2.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }
            
            }
            else if(network==2){
              //switch broadcast arp request
            
              string destination_Mac=s2.broadcast_Arp(DestinationIp,r2,network);
              // sender updates its arp cache
              devices[sender-1].arp_cache(r2.IP2,destination_Mac);
              cout<<"Updated ARP cache :"<<endl;
              devices[sender-1].print_ArpCache();
              //print routing table 
              r2.Routing_Table(r1,2);
              cout<<endl;
              r2.Print_Routing_Table(network);
              //check ARP CACHE of router
              r2.arp_cache(r2.IP1,r2.MAC1);
              r2.arp_cache(r2.IP2,r2.MAC2);
              r2.arp_cache(r1.IP2,r2.MAC2);
              cout<<endl;
              r2.print_ArpCache(network);
              cout<<endl;
              //traverse through routing table and check for NID that matches destination ip
              r2.routing_decision(DestinationIp);
             
              r1.Routing_Table(r2,1);
              cout<<endl;
              r1.Print_Routing_Table(1);
               //check ARP CACHE of router 1
              r1.arp_cache(r1.IP1,r1.MAC1);
              r1.arp_cache(r1.IP2,r1.MAC2);
              r1.arp_cache(r2.IP1,r2.MAC1);
              cout<<endl;
              r1.print_ArpCache(1);
              cout<<endl;
              cout<<"Router 1 sends ARP request "<<endl;
              cout<<endl;
              //switch will broadcast arp request 
              destination_Mac=s1.broadcast_Arp(DestinationIp,r,2);
              //sender updates its arp cache
              devices[sender-1].arp_cache(DestinationIp,destination_Mac);
               cout<<endl;
               sleep(4);
               cout<<"Log of TCP Packets sent from client to server"<<endl;
              cout<<endl;
              cout<<"Protocol used : Selective Repeat"<<endl;
              cout<<endl;
              end.Selective_Repeat();
              cout<<endl;

              s1.sendMessage(devices[sender-1],DestinationIp);
              cout<<endl;
              if (destinationPort == 80) {
                    end.http();
                } 
              else {
                    end.dns();
              }
            }
          }
      }
  
     }
     //dynamic routing
    else if(scheme==2){
         int enter;
         cout<<endl;
         
         cout<<endl;
         
       
        cout<<"Protocol Used : RIP (Routing Information Protocol)"<<endl;
        cout<<endl;
          //RIP
          //configure routers
         
          int numVertices;
          std::cout << "Enter the number of Routers: ";
          std::cin >> numVertices;
          if(numVertices>15){
            cout<<"Maximim Hop Count in RIP is 15"<<endl;
            cout<<"Enter a valid number "<<endl;
            continue;
          }
         
         vector<pair<Router,Router>> routers;
         vector<vector<int>> edges;
          int numEdges;
          cout<<endl;
          std::cout << "Enter the number of links : "<<endl;
          cin >> numEdges;
          cout<<endl;
          
          cout<<endl;
          cout<<"Input router number as per 0 based indexing"<<endl;
          for (int i = 0; i < numEdges; ++i) {
              
              int  source,destination,wt;
              
              cout << "Edge " << i + 1 << ":\n";
              cout << "First Router: ";
              cin>>source;
              Router r1(source);
             
              cout << "Second Router: ";
              cin >> destination;
              Router r2(destination);
              
              routers.push_back({r1,r2});
            }
            for(auto it:routers){
              vector<int> temp;
              temp={it.first.getId(),it.second.getId(),1};
              edges.push_back(temp);
              temp.clear();
              temp={it.second.getId(),it.first.getId(),1};
              edges.push_back(temp);
            }
            
           Router r;
           r.initial_Routing_table(edges,numVertices);
           cout<<endl;
            for (int source = 0; source < numVertices; ++source) {
                r.RIP(edges, numVertices, source);
            }

            
         
    }  
    else{
      cout<<"Invalid Choice"<<endl;
      continue;
    }
       break; 
     }
    
   
 
  
  }
};

class prompt{
   public:
  
   void run(){
    while(true){
    cout<<endl;
    int choice,hubs,switches,both;
     map<int,string> choose;
    
    choose[0]="Hub";
    choose[1]="Switch";
    choose[2]="Router";
    cout<<"Choose a device "<<endl;
    
    for(int i=0;i<choose.size();i++){
      cout<<i+1<<": "<<choose[i]<<endl;
    }
    cout<<endl;
    
    cin>>choice;
    
    switch (choice)
    {
    case 1:
    {
      cout<<endl;
      cout<<"Enter the number of hubs required"<<endl;
      cout<<endl;
      cin>>hubs;
      if(hubs==1){
      physical_prompt p;
      p.run();

      break;
      }
      else{
        //mutiple hubs 
        data_prompt d;
        d.run(2,hubs);
        break;
      }
    }
    case 2:
    {
      data_prompt d;
      d.run(1,hubs);
      break;
    }
    case 3:
    {
      network_prompt n;
      n.run();
      break;
    }
    default:
    cout<<"Invalid Entry"<<endl;
      break;
    }
    
    }
   
   }
};
  

