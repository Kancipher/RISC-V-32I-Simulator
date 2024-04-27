import tkinter as tk
from tkinter import filedialog
import subprocess

def launch_cpp_process():
    process = subprocess.Popen(["./RISCV_Instructions.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    return process

def get_cpp_output(process, code, data, pc):
    inputs = f"{code}\n{data}\n{pc}\n"  
    stdout, stderr = process.communicate(input=inputs)
    return stdout

def parse_input():
    code = input1.get()
    data = input2.get()
    pc = input3.get()
    process = launch_cpp_process()
    output = get_cpp_output(process, code, data, pc)
    output1.delete("1.0", tk.END)
    output1.insert(tk.END, output)

RISC_V = tk.Tk()
RISC_V.title("RISCV Simulator")

label1 = tk.Label(RISC_V, text="Enter the assembly code path in txt:")
label1.pack()

input1 = tk.Entry(RISC_V, width=60)
input1.pack()

label2 = tk.Label(RISC_V, text="Enter the data needed for the code (if no data needed just add an empty txt file):")
label2.pack()

input2 = tk.Entry(RISC_V, width=60)
input2.pack()

label3 = tk.Label(RISC_V, text="Enter the program counter for the instruction to start with:")
label3.pack()

input3 = tk.Entry(RISC_V, width=60)
input3.pack()

parse = tk.Button(RISC_V, text="Parse Input", command=parse_input)
parse.pack()

output1 = tk.Text(RISC_V, height=50, width=600)
output1.pack()

RISC_V.mainloop()

