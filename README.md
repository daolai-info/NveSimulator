# A Simulator of Network Virtualization Enviroment


It includes:
##
* generate online virtual network embedding requests.

* perform virtual network embedding, including virtual node embedding and virtual link embedding.  

* generate online end-user requests on virtual network.

* simulate point-to-point video data transmission service on virtual network.

* output statistical network information about substrate network and virtual networks.
 

In our contribution, we realize three virtual link embedding approaches:

   *  1 : 1  (KouYou: 紅葉)
        One virtual link mapped one substrate data transport path

   *  1 : 2  (OuYou: 黄葉)
        One virtual link mapped two static substrate data transport path
        
   *  1 : 1 or 2 (KuroMame: 黒豆)
        One virtual link mapped one or two substrate data transport paths that can be reconfigure dynamically based on network status.

