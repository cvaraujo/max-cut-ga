import os
import subprocess

folder = ['instances', 'beasley_all']
algorithms = []
commands = []

for f in folder:
    instance = os.listdir(f)
    for inst in instance:
        commands.append("./GA df 500 50 2400 " + f + "/" + inst + " results_" + f + "/result_" + inst)

for c in commands:
   print(c)
   p = subprocess.Popen(c, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
   msg, err = p.communicate()
   if msg:
   	print(msg)
   print("OK!!")
