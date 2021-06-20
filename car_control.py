import pyb, sensor, image, time, math

enable_lens_corr = False
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...


uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

GRAYSCALE_THRESHOLD = [(0, 64)]
ROI = [40, 10, 80, 25]

while(True):
   find = 0
   img = sensor.snapshot()
   if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
   blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=ROI, merge=True)
   if blobs:
       largest_blob = 0
       most_pixels = 0
       for i in range(len(blobs)):
           if blobs[i].pixels() > most_pixels:
               most_pixels = blobs[i].pixels()
               largest_blob = i
       img.draw_rectangle(blobs[largest_blob].rect())
       deflection_angle = math.atan((blobs[largest_blob].cx()-80)/30)
       deflection_angle = math.degrees(deflection_angle)
       #print(deflection_angle)
       #right:- left:+
       if abs(deflection_angle>20):
           if deflection_angle > 0:
               uart.write("/turn/run -50 0.1 \n".encode())
           else:
               uart.write("/turn/run -50 -0.1 \n".encode())
       else:
           uart.write("/goStraight/run -50 \n".encode())
       find = 1
   if find==0 :
       uart.write("/stop/run \n".encode())


