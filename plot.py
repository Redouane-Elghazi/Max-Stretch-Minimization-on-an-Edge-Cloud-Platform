import sys, os
from subprocess import *
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 16})
import json

experiment_name = "kang20-10"

#Gather data from simulations
"""
x, srpt, greedy, ssfedf, bender, T_srpt, T_greedy, T_ssfedf, T_bender = [], [], [], [], [], [], [], [], []
n = 100
ccr = 10
sample = 1000
Vmax = 20000
while n/1.6<=Vmax:
#while ccr/1.25<10:
	S_srpt, S_greedy, S_ssfedf, S_bender, t_srpt, t_greedy, t_ssfedf, t_bender = [float(k) for k in check_output(["./schedule_sim", str(n), str(sample), "0", str(ccr)]).split()]
	x += [n]
	#x += [ccr]
	srpt += [S_srpt]
	greedy += [S_greedy]
	ssfedf += [S_ssfedf]
	bender += [S_bender]
	T_srpt += [t_srpt]
	T_greedy += [t_greedy]
	T_ssfedf += [t_ssfedf]
	T_bender+= [t_bender]
	print(n, S_srpt, S_greedy, S_ssfedf, S_bender, t_srpt, t_greedy, t_ssfedf, t_bender)
	n = int(1.6*(n+1))
	#ccr = ccr*1.25
"""

#Use already gathered data
input_json = "plots/kang20-10/kang.json"
Vmax = 20000
with open(input_json, "r") as f:
	d = json.load(f)

x = d["x"]
srpt = d["srpt"]
greedy = d["greedy"]
ssfedf = d["ssfedf"]
bender = d["bender"]
T_srpt = d["T_srpt"]
T_greedy = d["T_greedy"]
T_ssfedf = d["T_ssfedf"]
T_bender = d["T_bender"]


plt.rc('text', usetex=True)
default = plt.rcParams["figure.figsize"]

plt.plot(x, srpt, label="{\em SRPT}")
plt.plot(x, greedy, label="{\em Greedy}")
plt.plot(x, ssfedf, label="{\em SSF-EDF}")
plt.plot(x, bender, label="{\em Edge-Only}")
plt.xscale("log")
plt.xlim(right=Vmax)
plt.ylim(bottom=1)
plt.ylabel('maximum stretch')
plt.xlabel('number of jobs')
#plt.xlabel('communication/computation ratio')
plt.savefig("{}_stretch.pdf".format(experiment_name), bbox_inches = 'tight',dpi=200)
plt.close()

plt.plot(x, srpt, label="{\em SRPT}")
plt.plot(x, greedy, label="{\em Greedy}")
plt.plot(x, ssfedf, label="{\em SSF-EDF}")
plt.xscale("log")
plt.xlim(right=Vmax)
plt.ylim(bottom=1)
plt.ylabel('maximum stretch')
plt.xlabel('number of jobs')
#plt.xlabel('communication/computation ratio')
plt.savefig("{}_stretch_nobender.pdf".format(experiment_name), bbox_inches = 'tight',dpi=200)
plt.close()

plt.plot(x, T_srpt, label="{\em SRPT}")
plt.plot(x, T_greedy, label="{\em Greedy}")
plt.plot(x, T_ssfedf, label="{\em SSF-EDF}")
plt.plot(x, T_bender, label="{\em Edge-Only}")
#plt.xscale("log")
plt.xlim(right=Vmax)
plt.ylabel('execution time in seconds')
plt.xlabel('number of jobs')
#plt.xlabel('communication/computation ratio')

ax = plt.gca()
handles, labels = ax.get_legend_handles_labels()
plt.savefig("{}_time.pdf".format(experiment_name), bbox_inches = 'tight',dpi=200)
plt.close()

for c in [1,2,4]:
	fig_legend = plt.figure(figsize=(0.1,0.1))
	axi = fig_legend.add_subplot(111)            
	fig_legend.legend(handles, labels, loc='center', ncol = c)
	axi.xaxis.set_visible(False)
	axi.yaxis.set_visible(False)
	fig_legend.canvas.draw()
	#plt.legend(handles, labels, loc='upper center', ncol = 4)
	plt.savefig("{}_legend_ncol{}.pdf".format(experiment_name,c), bbox_inches = 'tight',dpi=200)

d = {
		"x": x,
		"srpt": srpt,
		"greedy": greedy,
		"ssfedf": ssfedf,
		"bender": bender,
		"T_srpt": T_srpt,
		"T_greedy": T_greedy,
		"T_ssfedf": T_ssfedf,
		"T_bender": T_bender
	}

with open("{}.json".format(experiment_name), "w") as f:
	json.dump(d, f)