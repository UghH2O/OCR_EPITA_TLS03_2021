# tls-ocr-03
Projet S3 OCR
Edit: This project is not the original one but a copy to ensure privacy of other members who have participated in this project. Hugo ABADIE

Final result:
    After this final rendering it will be possible to compile the whole project
    but also to test the neural network separately.
    
    To test OCR application:
        - Go to root of git repository.
        - Execute the command "make".
        - Run the command "./main".
        - A window will open with several options:
        
        	- First of all it is proposed to choose the image on which
        	the operation will have to be carried out.
        	
        	- Once the image has been chosen, it is possible
        	to rotate the image according to several angles. 
        	(the straighter the image is, the better the result is).
        	
        	- Once the image is considered suitable,
        	pressing the "Start" button triggers character recognition.
        	
        	- It is possible to edit and save the output text,
        	displayed on the right, in a file named "text.txt".
        	
        	- It is also possible to train the neural network
        	by pressing the "train the neural network" button.
        	Triggering the training will close the window.
        	
        - It is possible to clean up the folder by running
        the "make clean" command from the root of the git repository.

        Several test images are available in the "images" folder:
            - lorem.PNG
            - noisy.png
            - rotlorem.PNG



    To test neural network:
    
        - Go to the repository "neural_network" of git repository,
        then in the subdirectory "OCR" or "XOR"
        depending on the intelligence you want to test.
        
        - Execute the command "make".
        - Run the command "./main".
        - It is possible to clean up the folder by running
        the "make clean" command from directory.
