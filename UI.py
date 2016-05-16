from Tkinter import *
import Tkinter as tk
import tkMessageBox
import ttk
import serial
import tkFileDialog

#----------------------------------------Variables-------------------------------------------------------

arduino=serial.Serial()
varString=""
count=0

#---------------------------------------Main window-------------------------------------------------------    
root=Tk()
root.resizable(width=FALSE, height=FALSE)
root.title("DMF Automation")   
#---------------------------------------Frames--------------------------------------------------------

type_frame=tk.LabelFrame(root, text="Command", labelanchor='n')
type_frame.grid(row=0, column=0, padx=10, pady=10)

dmf_frame=tk.LabelFrame(root, text="DMF Board", labelanchor='n')
dmf_frame.grid(row=0,column=1,columnspan=2)

algorithm_frame=tk.LabelFrame(root, text="Functions", labelanchor='n')
algorithm_frame.grid(row=0, column=3)

extra_frame=tk.LabelFrame(root)
extra_frame.grid(row=2,column=3, padx=10, pady=10)

history_frame=tk.LabelFrame(root, text="Serial Monitor", labelanchor='nw')
history_frame.grid(row=2, column=1, columnspan=2, padx=10, pady=10)

save_load=tk.LabelFrame(root, text="Save&Load", labelanchor="n")
save_load.grid(row=2, column=0, padx=10, pady=10) 
#----------------Function defenitions-----------------------------------------------------------------------------

def save_to_string(electrode_num): 
    global varString
    varString += (electrode_num+",")
    print "added ",electrode_num
    print varString
         
def send():
    if pop_up()==True:
        global varString
        varstring += "1002"
        print ("Sending...")
        arduino.write(varString)
        print "DONE!"
        varString=""
    
##def delete(self):
##    global varString
##    varString=last_entry
##    print varString

def pop_up():
    pop_up = tkMessageBox.askyesno(title="!!!", message="Are you sure?")
    if pop_up>0:
        return True
    else:
        return False
           
def Reset():
    if pop_up()==True:
        global varString
        varString = ""
        print "Reset!"
        
def Exit():
    if pop_up()==True:
        root.destroy()
        
def save():
    f = tkFileDialog.asksaveasfile(mode='w', defaultextension=".txt")
    if f is None: # asksaveasfile return `None` if dialog closed with "cancel".
        return
    f.write(varString)
    f.close() # `()` was missing.
    print "Sequence Saved!"

def load():
    global varString
    f=tkFileDialog.askopenfile(mode='r', defaultextension=".txt")
    if f is None:
        return
    varString=""
    print "Loaded Sequence!"
    while True:
        c = f.read(1)
        if not c:
            break
        varString += c
    print varString
    f.close()
    
    
#-----------------------------------Text box------------------------------------------------------------------        

text=tk.Text(history_frame)
text.grid(row=0, column=0,padx=10, pady=10)

#-----------------------------------Menus-------------------------------------------------------------------

menubar=Menu(root)
#File menu
File_menu=Menu(menubar,tearoff=0)
File_menu.add_command(label="Open", command=load)
File_menu.add_command(label="Save As...", command=save)
File_menu.add_command(label="Exit", command=Exit)
menubar.add_cascade(label="File",menu=File_menu)
#Help menu
Help_menu=Menu(menubar,tearoff=0)
Help_menu.add_command(label="Read Me")
Help_menu.add_command(label="About")
menubar.add_cascade(label="Help",menu=Help_menu)
root.config(menu=menubar)

#----------------------------------DMF buttons-------------------------------------------------------------

button1 = tk.Button(dmf_frame, text="1", command=lambda:save_to_string("1"))
button1.grid(column=0,row=0)

button2 = tk.Button(dmf_frame, text="2", command=lambda:save_to_string("2"))
button2.grid(column=1,row=0)

button3 = tk.Button(dmf_frame, text="3", command=lambda:save_to_string("3"))
button3.grid(column=2,row=0)

button4 = tk.Button(dmf_frame, text="4", command=lambda:save_to_string("4"))
button4.grid(column=3,row=0)

button5 = tk.Button(dmf_frame, text="5", command=lambda:save_to_string("5"))
button5.grid(column=4,row=0)

button6 = tk.Button(dmf_frame, text="6", command=lambda:save_to_string("6"))
button6.grid(column=0,row=1)

button7 = tk.Button(dmf_frame, text="7", command=lambda:save_to_string("7"))
button7.grid(column=1,row=1)

button8 = tk.Button(dmf_frame, text="8", command=lambda:save_to_string("8"))
button8.grid(column=2,row=1)

button9 = tk.Button(dmf_frame, text="9", command=lambda:save_to_string("9"))
button9.grid(column=3,row=1)

button10 = tk.Button(dmf_frame, text="10", command=lambda:save_to_string("10"))
button10.grid(column=4,row=1)

#--------------------------------------Type-----------------------------------------------------------------------

Seperate_button=ttk.Button(type_frame, text="Seperate", command=lambda: save_to_string("1001"))
Seperate_button.grid(row=0, column=0, sticky="nsew")

Sequence_button=ttk.Button(type_frame, text="Sequence", command=lambda: save_to_string("1000"))
Sequence_button.grid(row=1, column=0, sticky="nsew")

Mix_button=ttk.Button(type_frame, text="Mix", command=lambda: save_to_string("1003"))
Mix_button.grid(row=2, column=0, sticky="nsew")

#-----------------------------------Algorithms--------------------------------------------------
synbio_function_button= ttk.Button(algorithm_frame,text="Synbio function",command=lambda:save_to_string("S"))
synbio_function_button.grid(row=0,column=0)

DNA_assembly_button= ttk.Button(algorithm_frame,text=" DNA Assembly ",command=lambda:save_to_string("D"))
DNA_assembly_button.grid(row=1,column=0)

Transformation_button= ttk.Button(algorithm_frame,text="Transformation ",command=lambda:save_to_string("T") )
Transformation_button.grid(row=2,column=0)
#------------------------------------------Send, Delete ,Reset & Quit----------------------------------------------------------

##Delete_button=ttk.Button(extra_frame, text="Delete", command=delete_last_entry)
##Delete_button.grid(row=0, column=0, sticky="nsew")

Reset_button=ttk.Button(extra_frame, text="Reset Sequence", command=Reset)
Reset_button.grid(row=0, column=0, sticky="nsew")

Send_button= ttk.Button(extra_frame,text="Send Commands", command=send)
Send_button.grid(row=1, column=0, sticky="nsew")

END_button=ttk.Button(extra_frame, text="Quit", command=Exit)
END_button.grid(row=2, column=0, sticky="nsew")

#------------------------------------------save&load---------------------------------------------------------

save_button=ttk.Button(save_load, text="Save Sequence", command=save)
save_button.grid(row=0, column=0, sticky="nsew")

load_button=ttk.Button(save_load, text="Load Sequence", command=load)
load_button.grid(row=1, column=0, sticky="nsew")

#--------------------------------------------Bindings-------------------------------------------------------

root.bind('<Return>', send)
root.bind('<Control-r>', Reset)
#root.bind('<BackSpace>', delete)
root.bind('<Control-s>', save)
root.bind('<Control-o>', load)

#-------------------------------------------------------------------------------------------------
root.mainloop()
