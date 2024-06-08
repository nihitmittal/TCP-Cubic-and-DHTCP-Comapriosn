Tcp-analysis
Dumbbell topology is created in ns-3 simulator and the performance of both TCP versions - TCP Cubic and DCTCP in terms of throughput and average flow completion time is studied. The coexistence of these two protocols and its performance in terms of responding to the congestion in the network is also demonstrated.
Command to run the NS3 simulator: -
The code was built to execute on Ubuntu 22.04 LTS operating system. Open a new terminal and execute the below commans to build and run the NS3 simulator.
1. ./waf --build scratch/Code.cc or ./ns3 build scratch/Code.cc for later the newer versions of ns3
2. ./waf --run scratch/Code.cc or ./ns3 run scratch/Code.cc for later the newer versions of ns3
Remarks regarding implementation
Configuration
* S1 will send traffic to D1 only.
* S2 will send traffic to D2 only.
* All the servers i.e., S1, S2, D1, and D2 should be configured to support TCP Cubic (also known as TCP BIC in ns-3) and DCTCP.

The following points have been implemented in the code.
1. Each link in the network has a maximum bandwidth of 1 Gbps.
2. The file to be transferred is of size 50MB.
3. Six nodes - two senders, two routers and two receivers were built using the dumbbell topology.
Packages and Dependencies
Below are the packages used to build the network.
* "iostream"
* "fstream"
* "iomanip"
* "iterator"
* "ns3/core-module.h"
* "ns3/network-module.h"
* "ns3/internet-module.h"
* "ns3/point-to-point-module.h"
* "ns3/point-to-point-dumbbell.h"
* "ns3/applications-module.h"
* "ns3/packet-sink.h"
* "ns3/traffic-control-module.h"
* "ns3/bulk-send-helper.h"
* "ns3/bulk-send-application.h"
* "ns3/flow-monitor-module.h"
Experiments
The following experiments were performed using bulk sender to send 50 MB of data from source to destination.
* Exp-1: S1 sends traffic to D1 using TCP Cubic. S2 and D2 are not used in this experiment.
* Exp-2: S1 sends traffic to D1 and S2 sends traffic to D2. Both senders will use TCP Cubic and start sending data to respective destinations simultaneously.
* Exp-3: S1 sends traffic to D1 using DCTCP. S2 and D2 are not used in this experiment.
* Exp-4: S1 sends traffic to D1 and S2 sends traffic to D2. Both senders will use DCTCP and start sending data to respective destinations simultaneously.
* Exp-5: S1 sends traffic to D1 using TCP Cubic whereas S2 sends traffic to D2 using DCTCP. Both senders will start sending data to respective destinations simultaneously.

