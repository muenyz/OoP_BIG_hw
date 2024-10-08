import time
import pyautogui
import win32api
import math
pi=3.14159
mode=0

with open('a.txt','w') as file:
    x=[0,0,0,0]
    y=[0,0,0,0]
    angle=[0,0]
    cnt=0
    def getpos(i):  #取第i个点的坐标
        state_left = win32api.GetKeyState(0x01)  # Left button down = 0 or 1. Button up = -127 or -128
        while True:
            b=win32api.GetKeyState(0x02)
            if b<0:
                file.close()
                exit()
            a=win32api.GetKeyState(0x01)
            if a!= state_left:
                state_left=a
                if a<0:
                    x[i],y[i]=pyautogui.position()
                else:
                    break
        time.sleep(0.001)

    def calarc():
        a=x[1]-x[2]
        b=y[1]-y[2]
        c=x[1]-x[3]
        d=y[1]-y[3]
        e=((x[1]*x[1]-x[2]*x[2])-(y[2]*y[2]-y[1]*y[1]))/2
        f=((x[1]*x[1]-x[3]*x[3])-(y[3]*y[3]-y[1]*y[1]))/2
        pointx=round((e*d-b*f)/(a*d-b*c))
        pointy=round((a*f-e*c)/(a*d-b*c))
        radius=round(math.sqrt((x[1]-pointx)*(x[1]-pointx)+(y[1]-pointy)*(y[1]-pointy)))
        angle[0]=round(180*math.acos((pointy-y[1])/radius)/pi)
        if x[1]<pointx:
            angle[0]=360-angle[0]
        angle[1] = round(180*math.acos((pointy-y[3])/radius)/pi)
        if x[3] < pointx:
            angle[1] = 360 - angle[1]
        if (angle[1]-angle[0]+360)%360>180:
            temp=angle[1]
            angle[1]=angle[0]
            angle[0]=temp
        file.write('hdc_arc('+str(pointx-x[0])+','+str(pointy-y[0])+','+str(radius)+','+str(angle[0])+','+str(angle[1])+',1,0x00dddddd);\n')

    def caltgl():
        file.write('hdc_triangle('+str(x[1]-x[0])+','+str(y[1]-y[0])+','+str(x[2]-x[0])+','+str(y[2]-y[0])+','+str(x[3]-x[0])+','+str(y[3]-y[0])+',1,1,0x00dddddd);\n')

    getpos(0)
    while 1 :
        cnt+=1
        getpos(cnt%3+(0 if cnt%3 else 3))
        if cnt%3==0:
            if mode==1:
                calarc()
            elif mode==0:
                caltgl()
