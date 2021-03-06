關於本項目  
&emsp;&emsp;本項目分為三個部分，追蹤線、圍繞物體轉圈和識別AprilTag.  
&emsp;&emsp;&emsp;&emsp;1.追蹤線部分，小車根據初始位置時openMV識別出的黑線方向來判斷左轉或右轉，此步驟只做一次.  
&emsp;&emsp;&emsp;&emsp;2.圍繞物體轉圈部分，即小車繞一個物體完整繞一圈.  
&emsp;&emsp;&emsp;&emsp;3.識別AprilTag部分，小車對準AprilTag，識別出AprilTag並印出ping.  
&emsp;&emsp;每個部分開始和結束時Xbee_host會印出相應信息.  

1.how to setup and run your program  
&emsp;&emsp;(1)Clone the repo: $ git clone https://github.com/Lyds099/final.git  
&emsp;&emsp;(2) $ cd final_project  
&emsp;&emsp;(3)Compile the program: $ sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f  
&emsp;&emsp;(4)Open the screen for main.cpp: $ sudo screen /dev/ttyACM0  
&emsp;&emsp;(5)Execute xbee.py: $ sudo python3 xbee.py  
&emsp;&emsp;(6)Put the line before the car to steer the car to turn left/right.  
&emsp;&emsp;(7)After turning left/right, the car will circle around the object. Then put the AprilTag before the car, openMV will identify the AprilTag.  

2.what are the results  
&emsp;&emsp;openMV find the line: https://github.com/Lyds099/final/blob/master/Screen%20Shot%202021-06-23%20at%209.46.45%20AM.png?raw=true  
&emsp;&emsp;identify the AprilTag: https://drive.google.com/file/d/1QP9KwP7cGLBI8IrixVHnC6SwRH2BYodT/view?usp=sharing  
&emsp;&emsp;message received by xbee_host: https://github.com/Lyds099/final/blob/master/Screen%20Shot%202021-06-22%20at%209.50.13%20PM.png?raw=true  
&emsp;&emsp;printed ping value: https://github.com/Lyds099/final/blob/master/Screen%20Shot%202021-06-23%20at%2010.43.22%20AM.png?raw=true  
&emsp;&emsp;demo video(left): https://drive.google.com/file/d/1DWOOF1leqN_l3N6XEvbYsNFd9uKaKFah/view?usp=sharing  
&emsp;&emsp;demo video(right): https://drive.google.com/file/d/1lXGTvTD_hyYvb4Fm0LocXLSm9hR2x4DT/view?usp=sharing  
