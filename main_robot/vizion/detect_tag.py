import cv2

# Sélection du dictionnaire ArUco utilisé
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_100)
# Paramètres du détecteur
aruco_params = cv2.aruco.DetectorParameters()
# Créer le détecteur
detector = cv2.aruco.ArucoDetector(aruco_dict, aruco_params)

tags = ((),)
current_frame = None
frame_count = 0

def open_camera():
    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    if not cap.isOpened():
        print("❌ Impossible to open camera")
        return
    print("✅ Camera opened succesfully")

    return cap

def capture_frame(cap):
    global current_frame, frame_count
    frame_count += 1
    current_frame = cap.read()[1]
    current_frame = cv2.cvtColor(current_frame, cv2.COLOR_BGR2GRAY)

def import_frame_from_file(path):
    global current_frame, frame_count
    frame_count += 1
    current_frame = cv2.imread(path)
    current_frame = cv2.cvtColor(current_frame, cv2.COLOR_BGR2GRAY)


def find_tags():
    global current_frame, tags
    tags = detector.detectMarkers(current_frame)[0:2]


def debug_show_found_tags(path=""):
    global tags, current_frame
    print(tags)
    if tags == ((),):
        return
    cv2.aruco.drawDetectedMarkers(current_frame, *tags)

    if len(path):
        cv2.imwrite(path, current_frame)
    else:
        print("Ouverture de la fenêtre d'affichage")
        cv2.imshow("Flux camera", current_frame)
        cv2.waitKey(1)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        print("Fermeture de la fenêtre d'affichage")

if __name__ == "__main__":
    capture_mode = input("Capture mode?(y/n)")=='y'
    cap = None
    if capture_mode:
        cap = open_camera()
        capture_frame(cap)
    else:
        import_frame_from_file(input("Absolute file path>"))
    find_tags()
    debug_show_found_tags(input("Out path>"))
    
    if cap != None:
        cap.release()

