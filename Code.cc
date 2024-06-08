#include<iostream>
#include<fstream>
#include<string>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-dumbbell.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink.h"
#include "ns3/flow-monitor-module.h"

//#include "ns3/simulator-module.h"
//#include "ns3/node-module.h"
//#include "ns3/helper-module.h"

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE("test");

void getStats (vector<double> arr, double *mean, double *std){ //a function to just calculate mean and stddev

  for (int i=0; i<3; i++) {
    *mean += arr[i];
  }
  *mean = (double) *mean / 3;
 
  for (int i=0; i<3; ++i)
    *std += pow (arr[i]-*mean, 2);
  *std = sqrt(*std / 3.0);
}

int main(int argc, char *argv[]){
  uint32_t maxBytes = 50000000;
  Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpBic"));

  //setup helpers for experiment 1 and 2
  P2PHelper pointLeft, pointRight, pointRightouter;
  pointLeft.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRight.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRightouter.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  
  P2PDumbbellHelper pointDumbbell = P2PDumbbellHelper(2, pointLeft, 2, pointRight, pointRightouter);

  InternetStackHelper stack;
  pointDumbbell.InstallStack(stack);

  Ipv4AddressHelper IPL, IPR, IPRouter;
  IPL.SetBase("10.1.1.0", "255.255.255.252");
  IPR.SetBase("10.1.2.0", "255.255.255.252");
  IPRouter.SetBase("10.1.0.0", "255.255.255.240");

  pointDumbbell.AssignIpv4Addresses(IPL, IPR, IPRouter);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 80));
  BulkSendHelper client1("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell.GetRightIpv4Address(0), 80));
  BulkSendHelper client2("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell.GetRightIpv4Address(1), 80));
  client1.SetAttribute("MaxBytes", UintegerValue(maxBytes));
  client2.SetAttribute("MaxBytes", UintegerValue(maxBytes));


  //experiment 1
  ApplicationContainer server1 = sinkHelper.Install(pointDumbbell.GetRight(0));
  server1.Start(Seconds (1.0));
  server1.Stop(Seconds (30.0));

  ApplicationContainer send1 = client1.Install(pointDumbbell.GetLeft(0));
  send1.Start(Seconds(1.0));
  send1.Stop(Seconds(10.0));

  ApplicationContainer send2 = client1.Install(pointDumbbell.GetLeft(0));
  send2.Start(Seconds(11.0));
  send2.Stop(Seconds(20.0));

  ApplicationContainer send3 = client1.Install(pointDumbbell.GetLeft(0));
  send3.Start(Seconds(21.0));
  send3.Stop(Seconds(30.0));


  //experiment 2
  ApplicationContainer server21 = sinkHelper.Install(pointDumbbell.GetRight(0));
  ApplicationContainer server22 = sinkHelper.Install(pointDumbbell.GetRight(1));
  server21.Start(Seconds (31.0));
  server21.Stop(Seconds (60.0));
  server22.Start(Seconds (31.0));
  server22.Stop(Seconds (60.0));

  ApplicationContainer send21 = client1.Install(pointDumbbell.GetLeft(0));
  send21.Start(Seconds(31.0));
  send21.Stop(Seconds(40.0));

  ApplicationContainer send22 = client1.Install(pointDumbbell.GetLeft(0));
  send22.Start(Seconds(41.0));
  send22.Stop(Seconds(50.0));

  ApplicationContainer send23 = client1.Install(pointDumbbell.GetLeft(0));
  send23.Start(Seconds(51.0));
  send23.Stop(Seconds(60.0));

  ApplicationContainer send31 = client2.Install(pointDumbbell.GetLeft(1));
  send31.Start(Seconds(31.0));
  send31.Stop(Seconds(40.0));

  ApplicationContainer send32 = client2.Install(pointDumbbell.GetLeft(1));
  send32.Start(Seconds(41.0));
  send32.Stop(Seconds(50.0));

  ApplicationContainer send33 = client2.Install(pointDumbbell.GetLeft(1));
  send33.Start(Seconds(51.0));
  send33.Stop(Seconds(60.0));

  //setup helpers for experiment 3 and 4
  Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpDctcp"));
  P2PHelper pointLeft2, pointRight2, pointRightouter2;
  pointLeft2.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRight2.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRightouter2.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  
  P2PDumbbellHelper pointDumbbell2 = P2PDumbbellHelper(2, pointLeft2, 2, pointRight2, pointRightouter2);

  InternetStackHelper stack2;
  pointDumbbell2.InstallStack(stack2);

  Ipv4AddressHelper IPL2, IPR2, IPRouter2;
  IPL2.SetBase("10.1.4.0", "255.255.255.252");
  IPR2.SetBase("10.1.5.0", "255.255.255.252");
  IPRouter2.SetBase("10.1.3.0", "255.255.255.240");

  pointDumbbell2.AssignIpv4Addresses(IPL2, IPR2, IPRouter2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  PacketSinkHelper sinkHelper2("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 81));
  BulkSendHelper client12("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell2.GetRightIpv4Address(0), 81));
  BulkSendHelper client22("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell2.GetRightIpv4Address(1), 81));
  client12.SetAttribute("MaxBytes", UintegerValue(maxBytes));
  client22.SetAttribute("MaxBytes", UintegerValue(maxBytes));

  //experiment 3
  ApplicationContainer server_e3 = sinkHelper2.Install(pointDumbbell2.GetRight(0));
  server_e3.Start(Seconds (61.0));
  server_e3.Stop(Seconds (90.0));

  ApplicationContainer send_e31 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e31.Start(Seconds(61.0));
  send_e31.Stop(Seconds(70.0));

  ApplicationContainer send_e32 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e32.Start(Seconds(71.0));
  send_e32.Stop(Seconds(80.0));

  ApplicationContainer send_e33 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e33.Start(Seconds(81.0));
  send_e33.Stop(Seconds(90.0));

  //experiment 4
  ApplicationContainer server_e41 = sinkHelper2.Install(pointDumbbell2.GetRight(0));
  ApplicationContainer server_e42 = sinkHelper2.Install(pointDumbbell2.GetRight(1));
  server_e41.Start(Seconds (91.0));
  server_e41.Stop(Seconds (120.0));
  server_e42.Start(Seconds (91.0));
  server_e42.Stop(Seconds (120.0));

  ApplicationContainer send_e41 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e41.Start(Seconds(91.0));
  send_e41.Stop(Seconds(100.0));

  ApplicationContainer send_e42 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e42.Start(Seconds(101.0));
  send_e42.Stop(Seconds(110.0));

  ApplicationContainer send_e43 = client12.Install(pointDumbbell2.GetLeft(0));
  send_e43.Start(Seconds(111.0));
  send_e43.Stop(Seconds(120.0));

  ApplicationContainer send_e44 = client22.Install(pointDumbbell2.GetLeft(1));
  send_e44.Start(Seconds(91.0));
  send_e44.Stop(Seconds(100.0));

  ApplicationContainer send_e45 = client22.Install(pointDumbbell2.GetLeft(1));
  send_e45.Start(Seconds(101.0));
  send_e45.Stop(Seconds(110.0));

  ApplicationContainer send_e46 = client22.Install(pointDumbbell2.GetLeft(1));
  send_e46.Start(Seconds(111.0));
  send_e46.Stop(Seconds(120.0));

  //setup helpers for experiment 5
  P2PHelper pointLeft3, pointRight3, pointRightouter3;
  pointLeft3.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRight3.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  pointRightouter3.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
  
  P2PDumbbellHelper pointDumbbell3 = P2PDumbbellHelper(2, pointLeft3, 2, pointRight3, pointRightouter3);

  Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpBic"));
  InternetStackHelper stack3;
  stack3.Install(pointDumbbell3.GetLeft(0));
  stack3.Install(pointDumbbell3.GetRight(0));

  Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpDctcp"));
  InternetStackHelper stack4;
  stack4.Install(pointDumbbell3.GetLeft(1));
  stack4.Install(pointDumbbell3.GetRight(1));

  stack4.Install(pointDumbbell3.GetLeft());
  stack4.Install(pointDumbbell3.GetRight());

  Ipv4AddressHelper IPL3, IPR3, IPRouter3;
  IPL3.SetBase("10.1.7.0", "255.255.255.252");
  IPR3.SetBase("10.1.8.0", "255.255.255.252");
  IPRouter3.SetBase("10.1.6.0", "255.255.255.240");

  pointDumbbell3.AssignIpv4Addresses(IPL3, IPR3, IPRouter3);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  PacketSinkHelper sinkHelper3("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 81));
  BulkSendHelper client13("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell3.GetRightIpv4Address(0), 81));
  BulkSendHelper client23("ns3::TcpSocketFactory", InetSocketAddress(pointDumbbell3.GetRightIpv4Address(1), 81));
  client13.SetAttribute("MaxBytes", UintegerValue(maxBytes));
  client23.SetAttribute("MaxBytes", UintegerValue(maxBytes));

  //Experiment 5
  ApplicationContainer server_e51 = sinkHelper3.Install(pointDumbbell3.GetRight(0));
  ApplicationContainer server_e52 = sinkHelper3.Install(pointDumbbell3.GetRight(1));
  server_e51.Start(Seconds (121.0));
  server_e51.Stop(Seconds (150.0));
  server_e52.Start(Seconds (121.0));
  server_e52.Stop(Seconds (150.0));

  ApplicationContainer send_e51 = client13.Install(pointDumbbell3.GetLeft(0));
  send_e51.Start(Seconds(121.0));
  send_e51.Stop(Seconds(130.0));

  ApplicationContainer send_e52 = client13.Install(pointDumbbell3.GetLeft(0));
  send_e52.Start(Seconds(131.0));
  send_e52.Stop(Seconds(140.0));

  ApplicationContainer send_e53 = client13.Install(pointDumbbell3.GetLeft(0));
  send_e53.Start(Seconds(141.0));
  send_e53.Stop(Seconds(150.0));

  ApplicationContainer send_e54 = client23.Install(pointDumbbell3.GetLeft(1));
  send_e54.Start(Seconds(121.0));
  send_e54.Stop(Seconds(130.0));

  ApplicationContainer send_e55 = client23.Install(pointDumbbell3.GetLeft(1));
  send_e55.Start(Seconds(131.0));
  send_e55.Stop(Seconds(140.0));

  ApplicationContainer send_e56 = client23.Install(pointDumbbell3.GetLeft(1));
  send_e56.Start(Seconds(141.0));
  send_e56.Stop(Seconds(150.0));

  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  NS_LOG_INFO("Run Simulator");
  Simulator::Stop(Seconds(150.0));
  Simulator::Run();
  Simulator::Destroy();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());
std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
  int flow_count = 1;
  vector<double> exp_1_thr; //array to store Throughput for 3 runs of exp1
  vector<double> exp_1_ft; //array to store Flow completion time for 3 runs of exp1

vector<double> exp_2_thr_sender1;
vector<double> exp_2_thr_sender2;
vector<double> exp_2_ft_sender1;
vector<double> exp_2_ft_sender2;

vector<double> exp_3_thr; //array to store Throughput for 3 runs of exp1
vector<double> exp_3_ft; //array to store Flow completion time for 3 runs of exp1

vector<double> exp_4_thr_sender1;
vector<double> exp_4_thr_sender2;
vector<double> exp_4_ft_sender1;
vector<double> exp_4_ft_sender2;

vector<double> exp_5_thr_sender1;
vector<double> exp_5_thr_sender2;
vector<double> exp_5_ft_sender1;
vector<double> exp_5_ft_sender2;

string th_1 = "";
string th_2 = "";
string th_22 = "";
string th_3 = "";
string th_4 = "";
string th_42 = "";
string th_5 = "";
string th_52 = "";
string afct_1 = "";
string afct_2 = "";
string afct_22 = "";
string afct_3 = "";
string afct_4 = "";
string afct_42 = "";
string afct_5 = "";
string afct_52 = "";

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
	/*
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      	  std::cout << "  Throughput: "<< i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
*/
	
	double thr = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024;
        //std::cout<<"Thr: "<<thr<<"\n";
	 
	double fct = i->second.timeLastTxPacket.GetSeconds () - i->second.timeFirstTxPacket.GetSeconds (); //FCT refers to flow completion time
        //cout<<"fct: "<<fct<<endl;

/*
//uncomment for exp1
	if (flow_count % 2 == 1 ){ //note that odd outputs are file transfer between source and destination and even outputs are file transfer between destination and source (sending of ACK packets), hence we skip even outputs
		exp_1_thr.push_back(thr);
	        exp_1_ft.push_back(fct)	; 
	} 
*/

	//experiment 1
        if(flow_count % 2 == 1 &&  flow_count <= 6){
		exp_1_thr.push_back(thr);
	        exp_1_ft.push_back(fct)	;
		th_1 = th_1 + to_string(thr) + ",";
		afct_1 = afct_1 + to_string(fct) + ",";
	}
	//experiment 2
        else if ((flow_count-6)%4==1 && flow_count <= 18){
                exp_2_thr_sender1.push_back(thr);
                exp_2_ft_sender1.push_back(fct);
		th_2 = th_2 + to_string(thr) + ",";
		afct_2 = afct_2 + to_string(fct) + ",";
	}
        else if ((flow_count-6)%4==2 && flow_count <= 18){
                exp_2_thr_sender2.push_back(thr);
                exp_2_ft_sender2.push_back(fct);
		th_22 = th_22 + to_string(thr) + ",";
		afct_22 = afct_22 + to_string(fct) + ",";
	}
	//experiment 3
        else if((flow_count-18)%2==1 && flow_count <= 24){
		exp_3_thr.push_back(thr);
	        exp_3_ft.push_back(fct)	; 
		th_3 = th_3 + to_string(thr) + ",";
		afct_3 = afct_3 + to_string(fct) + ",";
	}
	//experiment 4
        else if((flow_count-24)%4==1 && flow_count <= 36){
		exp_4_thr_sender1.push_back(thr);
	        exp_4_ft_sender1.push_back(fct)	; 
		th_4 = th_4 + to_string(thr) + ",";
		afct_4 = afct_4 + to_string(fct) + ",";
	}
        else if((flow_count-24)%4==2 && flow_count <= 36){
		exp_4_thr_sender2.push_back(thr);
	        exp_4_ft_sender2.push_back(fct)	; 
		th_42 = th_42 + to_string(thr) + ",";
		afct_42 = afct_42 + to_string(fct) + ",";
	}
	//experiment 5
        else if((flow_count-36)%4==1){
		exp_5_thr_sender1.push_back(thr);
	        exp_5_ft_sender1.push_back(fct)	; 
		th_5 = th_5 + to_string(thr) + ",";
		afct_5 = afct_5 + to_string(fct) + ",";
	}
        else if((flow_count-36)%4==2){
		exp_5_thr_sender2.push_back(thr);
	        exp_5_ft_sender2.push_back(fct)	; 
		th_52 = th_52 + to_string(thr) + ",";
		afct_52 = afct_52 + to_string(fct) + ",";
	}


        flow_count++;
}

double Tmean = 0, Tstd = 0;
double Fmean = 0, Fstd = 0;
getStats(exp_1_thr, &Tmean, &Tstd);
getStats(exp_1_ft, &Fmean, &Fstd);

double T1mean = 0, T1std = 0, T2mean = 0, T2std = 0;
double F1mean = 0, F1std = 0, F2mean = 0, F2std = 0;
getStats(exp_2_thr_sender1, &T1mean, &T1std);
getStats(exp_2_ft_sender1, &F1mean, &F1std);
getStats(exp_2_thr_sender2, &T2mean, &T2std);
getStats(exp_2_ft_sender2, &F2mean, &F2std);

double T3mean = 0, T3std = 0;
double F3mean = 0, F3std = 0;
getStats(exp_3_thr, &T3mean, &T3std);
getStats(exp_3_ft, &F3mean, &F3std);

double T4mean = 0, T4std = 0, T5mean = 0, T5std = 0;
double F4mean = 0, F4std = 0, F5mean = 0, F5std = 0;
getStats(exp_4_thr_sender1, &T4mean, &T4std);
getStats(exp_4_ft_sender1, &F4mean, &F4std);
getStats(exp_4_thr_sender2, &T5mean, &T5std);
getStats(exp_4_ft_sender2, &F5mean, &F5std);

double T6mean = 0, T6std = 0, T7mean = 0, T7std = 0;
double F6mean = 0, F6std = 0, F7mean = 0, F7std = 0;
getStats(exp_5_thr_sender1, &T6mean, &T6std);
getStats(exp_5_ft_sender1, &F6mean, &F6std);
getStats(exp_5_thr_sender2, &T7mean, &T7std);
getStats(exp_5_ft_sender2, &F7mean, &F7std);

/*
cout << "Exp 1 Throughput mean :" <<Tmean << " and Throughput std: " << Tstd << endl;
cout << "Exp 1 FCT mean :" <<Fmean << " and FCT std: " << Fstd << endl;

cout << "Exp 2 Throughput mean for sender 1:" <<T1mean << " and Throughput std for sender 1: " << T1std << endl;
cout << "Exp 2 FCT mean for sender 1:" <<F1mean << " and FCT std for sender 2: " << F1std << endl;
cout << "Exp 2 Throughput mean for sender 2:" <<T2mean << " and Throughput std for sender 2: " << T2std << endl;
cout << "Exp 2FCT mean for sender 2:" <<F2mean << " and FCT std for sender 2: " << F2std << endl;

cout << "Exp 3 Throughput mean :" <<T3mean << " and Throughput std: " << T3std << endl;
cout << "Exp 3 FCT mean :" <<F3mean << " and FCT std: " << F3std << endl;

cout << "Exp 4 Throughput mean for sender 1:" <<T4mean << " and Throughput std for sender 1: " << T4std << endl;
cout << "Exp 4 FCT mean for sender 1:" <<F4mean << " and FCT std for sender 2: " << F4std << endl;
cout << "Exp 4 Throughput mean for sender 2:" <<T5mean << " and Throughput std for sender 2: " << T5std << endl;
cout << "Exp 4 FCT mean for sender 2:" <<F5mean << " and FCT std for sender 2: " << F5std << endl;

cout << "Exp 5 Throughput mean for sender 1:" <<T6mean << " and Throughput std for sender 1: " << T6std << endl;
cout << "Exp 5 FCT mean for sender 1:" <<F6mean << " and FCT std for sender 2: " << F6std << endl;
cout << "Exp 5 Throughput mean for sender 2:" <<T7mean << " and Throughput std for sender 2: " << T7std << endl;
cout << "Exp 5 FCT mean for sender 2:" <<F7mean << " and FCT std for sender 2: " << F7std << endl;
*/

th_1 = "th_1," + th_1 + to_string(Tmean) + "," + to_string(Tstd) + ",Mbps\n";
afct_1 = "afct_1," + afct_1 + to_string(Fmean) + "," + to_string(Fstd) + ",sec\n";

th_2 = "th_2," + th_2 + to_string(T1mean) + "," + to_string(T1std) + ",Mbps,";
th_22 = th_22 + to_string(T2mean) + "," + to_string(T2std) + ",Mbps\n";
afct_2 = "afct_2," + afct_2 + to_string(F1mean) + "," + to_string(F1std) + ",sec,";
afct_22 = afct_22 + to_string(F2mean) + "," + to_string(F2std) + ",sec\n";

th_3 = "th_3," + th_3 + to_string(T3mean) + "," + to_string(T3std) + ",Mbps\n";
afct_3 = "afct_3," + afct_3 + to_string(F3mean) + "," + to_string(F3std) + ",sec\n";

th_4 = "th_4," + th_4 + to_string(T4mean) + "," + to_string(T4std) + ",Mbps,";
th_42 = th_42 + to_string(T5mean) + "," + to_string(T5std) + ",Mbps\n";
afct_4 = "afct_4," + afct_4 + to_string(F4mean) + "," + to_string(F4std) + ",sec,";
afct_42 = afct_42 + to_string(F5mean) + "," + to_string(F5std) + ",sec\n";

th_5 = "th_5," + th_5 + to_string(T6mean) + "," + to_string(T6std) + ",Mbps,";
th_52 = th_52 + to_string(T7mean) + "," + to_string(T7std) + ",Mbps\n";
afct_5 = "afct_5," + afct_5 + to_string(F6mean) + "," + to_string(F6std) + ",sec,";
afct_52 = afct_52 + to_string(F7mean) + "," + to_string(F7std) + ",sec\n";

string titles = "exp,r1_s1,r2_s1,r3_s1,avg_s1,std_s1,unit_s1,r1_s2,r2_s2,r3_s2,avg_s2,std_s2,unit_s2\n";
/*
cout<<"th"<<endl;
cout<<th_1<<endl;
cout<<th_2<<endl;
cout<<th_22<<endl;
cout<<th_3<<endl;
cout<<th_4<<endl;
cout<<th_42<<endl;
cout<<th_5<<endl;
cout<<th_52<<endl;
cout<<"afct"<<endl;
cout<<afct_1<<endl;
cout<<afct_2<<endl;
cout<<afct_22<<endl;
cout<<afct_3<<endl;
cout<<afct_4<<endl;
cout<<afct_42<<endl;
cout<<afct_5<<endl;
cout<<afct_52<<endl;
*/
ofstream tcpfile;
tcpfile.open("tcp_nmittal2.csv");
tcpfile<<titles;
tcpfile<<th_1;
tcpfile<<th_2;
tcpfile<<th_22;
tcpfile<<th_3;
tcpfile<<th_4;
tcpfile<<th_42;
tcpfile<<th_5;
tcpfile<<th_52;
tcpfile<<afct_1;
tcpfile<<afct_2;
tcpfile<<afct_22;
tcpfile<<afct_3;
tcpfile<<afct_4;
tcpfile<<afct_42;
tcpfile<<afct_5;
tcpfile<<afct_52;
tcpfile.close();

  

  flowMonitor->SerializeToXmlFile("testResults.xml", true, true);
  //out<<"go here 3\n";
  //Ptr<PacketSink> server = DynamicCast<PacketSink>(server1.Get(0));
  //cout<<"go here 4\n";
  //cout<<"Total Bytes Received: "<< server->GetTotalRx() <<endl;
  cout<<"finished\n";
  return 0;
}