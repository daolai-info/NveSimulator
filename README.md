# A Simulator of Network Virtualization Enviroment

we contribute a simulator of network virtualization environment. 
It includes:
  a).  generate online virtual network embedding request
  b).  perform virtual network embedding, including virtual node embedding and virtual link embedding
  c).  generate online end-user request of virtual network 
  d).  simulate point-to-point video data transmission service of virtual network.
  e).  output statistical network information about substrate network and virtual networks.


In our contribution, we realize three virtual link embedding approaches:
    I).  1 : 1  (KouYou: 紅葉)
         One virtual link mapped one substrate data transport path
   II).  1 : 2  (OuYou: 黄葉)
         One virtual link mapped two static substrate data transport path
  III).  1 : 1 or 2 (KuroMame: 黒豆)
         One virtual link mapped one or two substrate data transport paths that can be reconfigure dynamic based on network status.
