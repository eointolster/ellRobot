import cv2
from PIL import Image
import ell
import requests
import time

# ESP8266 IP address
esp_ip = "http://192.168.0.208"

# Commands for motors
commands = {
    "motor1_forward": "/M1_FORWARD",
    "motor1_stop": "/M1_STOP",
    "motor2_forward": "/M2_FORWARD",
    "motor2_stop": "/M2_STOP",
    "motor3_forward": "/M3_FORWARD",
    "motor3_stop": "/M3_STOP",
    "motor4_forward": "/M4_FORWARD",
    "motor4_stop": "/M4_STOP"
}

# Send motor commands to the robot
def send_command(command):
    try:
        url = f"{esp_ip}{commands[command]}"
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Command '{command}' sent successfully: {response.text}")
        else:
            print(f"Failed to send command '{command}'. Status code: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Error sending command: {e}")

# Movement functions
def move_forward():
    send_command("motor1_forward")
    send_command("motor2_forward")
    send_command("motor3_forward")
    send_command("motor4_forward")
    print("Moving forward")

def stop_vehicle():
    send_command("motor1_stop")
    send_command("motor2_stop")
    send_command("motor3_stop")
    send_command("motor4_stop")
    print("Stopping vehicle")

def turn_left():
    send_command("motor2_forward")
    send_command("motor3_forward")
    send_command("motor1_stop")
    send_command("motor4_stop")
    print("Turning left")

def turn_right():
    send_command("motor1_forward")
    send_command("motor4_forward")
    send_command("motor2_stop")
    send_command("motor3_stop")
    print("Turning right")

# Define the ell model to describe the activity
@ell.simple(model="gpt-4o", temperature=0.1)
def describe_activity(image: Image.Image):
    return [
        ell.system("You are VisionGPT. Answer <5 words all lower case."),
        ell.user(["Is there a girl doll with brown hair, large eyes, and wearing a dress in the image? It may resemble a teddy bear or soft toy.", image])
    ]

# Function to measure the size of the detected object (to check closeness)
def estimate_doll_size(image: Image.Image):
    # In a more sophisticated approach, you could use object detection and bounding boxes
    # Here, we will use a placeholder value, assuming the image size represents proximity
    width, height = image.size
    # Use a simple heuristic: if the height of the object takes up more than 50% of the frame, it's close
    return height > 240  # Assuming 480p resolution, 50% of 480 is 240 pixels

# Function to autonomously control the robot based on AI input
def autonomous_robot_control():
    print("Autonomous mode activated. Searching for the girl doll or teddy bear...")
    
    cap = cv2.VideoCapture(1)
    if not cap.isOpened():
        raise Exception("Could not open video device")
    
    print("Waiting for camera to initialize...")
    time.sleep(5)
    
    consecutive_detections = 0
    
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to capture image")
            continue

        cv2.imshow("Robot Camera Feed", frame)
        
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        pil_image = Image.fromarray(frame_rgb)

        description = describe_activity(pil_image)
        print(f"AI response: {description}")

        if "yes" in description.lower():
            consecutive_detections += 1
            print(f"Doll detected! (Detection {consecutive_detections}/2)")
            
            if consecutive_detections == 1:
                print("Moving forward.")
                move_forward()
                time.sleep(0.5)  # Move forward for half a second
                stop_vehicle()
            elif consecutive_detections >= 2:
                print("Doll detected twice. Staying still.")
                # Do nothing
        else:
            consecutive_detections = 0
            print("Doll not detected. Turning to search...")
            turn_right()
            time.sleep(0.25)  # Short turn duration
            stop_vehicle()

        time.sleep(1)  # Pause before next check

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

# Start autonomous robot control
autonomous_robot_control()
