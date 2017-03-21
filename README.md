# A Simulator of Network Virtualization Enviroment

Network virtualization is one of the most promising technologies for the future Internet [1], [2], [3].
We contribute a simulator of network virtualization environment. 

### It includes:

   - generate online virtual network embedding requests.

   - perform virtual network embedding, including virtual node embedding and virtual link embedding.  

   - generate online end-user requests on virtual network.

   - simulate point-to-point video data transmission service on virtual network.

   - output statistical network information about substrate network and virtual networks.
 


### In our contribution, we realize three virtual link embedding approaches:

   * 1 : 1  (KouYou: 紅葉)
   
        One virtual link mapped one substrate data transport path
        
   * 1 : 2  (OuYou: 黄葉)
   
        One virtual link mapped two static substrate data transport paths
   
   * 1 : 1 or 2 (KuroMame: 黒豆)
   
        One virtual link mapped one or two substrate data transport paths that can be reconfigure dynamically based on network status.


Update information at any time

#
[1] M. Chowdhury and R. Boutaba, “Network virtualization: State of the art and research challenges,” IEEE Commun. Mag., vol. 47, pp. 20–26, Jul. 2009. 

[2] N. M. K. Chowdhury and R. Boutaba, “A survey of network virtualization,” Comput. Netw., vol. 54, pp. 862–876, Apr. 2010. 

[3]. A. Fischer, J. F. Botero, M. T. Beck, H. Meer, and X. Hesselbach,“Virtual Network Embedding: A Survey,”   IEEE Communications Surveys & Tutorials, VOL. 15, NO. 4, FOURTH QUARTER 2013.
