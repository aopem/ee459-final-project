from picamera import PiCamera
import Rpi.GPIO as GPIO
import time

def main():
  atmega_signal = 4

  # set pin on RPI to wait for ATmega328P signal
  GPIO.setmode(GPIO.BOARD)
  GPIO.setup(atmega_signal, GPIO.IN)

  # initialize camera
  camera     = PiCamera()
  resolution = (1024, 768)

  # set resolution for camera
  camera.resolution = resolution

  while True:
    if GPIO.input(atmega_signal) == 1:    # check if atmega has sent a signal
      camera.start_recording("./videos")  # if high, record for 3 seconds
      time.sleep(3)                     
    else:
      camera.stop_recording()             # else do not record at all/stop recording

if __name__ == "__main__":
  main()