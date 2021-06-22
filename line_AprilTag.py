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

GRAYSCALE_THRESHOLD = [(64, 0)]
ROI = [30, 5, 100, 25]

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)
def degrees(radians):
   return (180 * radians) / math.pi

while(True):
   find = 0
   img = sensor.snapshot()
   if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...
   blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=ROI, merge=False)
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
               uart.write("l".encode())
           else:
               uart.write("r".encode())
       else:
           uart.write("o".encode())
       find = 1
   if find==0 :
       uart.write("n".encode())

   for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
      img.draw_rectangle(tag.rect(), color = (255, 0, 0))
      img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
      angle = degrees(tag.y_rotation())
      if ((angle <= 360 and angle >= 350) or (angle >= 0 and angle <= 10)):
          uart.write("t")
      #elif (angle <= 90 and angle >= 10):
          #uart.write("l")
      #elif (angle <= 350 and angle >= 270):
          #uart.write("r")



