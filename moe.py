#!/usr/bin/env python
import math
import random
from moe.easy_interface.experiment import Experiment
from moe.easy_interface.simple_endpoint import gp_next_points
from moe.optimal_learning.python.data_containers import SamplePoint
from subprocess import check_output
import re
import datetime


result = re.compile(r'^\s+(\d+)\s+heuristic\s*$', re.MULTILINE)

def function_to_minimize(x):
	
	print x
	
	# Write new config file
	with open("MoveHeuristic.conf.h", mode='w') as f:
		f.write("// Five dimensional tweak space, need to optimize with Yelps MOE\n")
		f.write("const float MoveHeuristic::improveMSTScore = 1.0; // Fixed for scale\n")
		f.write("const float MoveHeuristic::nonLeafPieceScore = " + str(x[0]) + ";\n")
		f.write("const float MoveHeuristic::opponentHinderScore = " + str(x[1]) + ";\n")
		f.write("const float MoveHeuristic::opponentBlockScore = " + str(x[2]) + ";\n")
		f.write("const float MoveHeuristic::freeNeighborScore = " + str(x[3]) + ";\n")
		f.write("const float MoveHeuristic::opponentNeighborScore = " + str(x[4]) + ";\n")
	
	# Compile and compete
	res = check_output("make competition 2>&1", shell=True)
	
	# Extract score
	score = float(result.search(res).group(1))
	print score
	
	# We want to maximize the score
	return -score


# 5D experiment, we build a tensor product domain
exp = Experiment([[-5, 5], [-5, 5], [-5, 5], [-5, 5], [-5, 5]])


# Bootstrap with some known or already sampled point(s)
exp.historical_data.append_sample_points([
	SamplePoint([-0.5, 0.3, 0.7, -0.4, 0.6], -224.0, 15.0),
	SamplePoint([0, 0, 0, 0, 0], -236, 15.0),
])

def itterate():
	print str(datetime.datetime.now())
	
	# Use MOE to determine what is the point with highest Expected Improvement to use next
	next_point_to_sample = gp_next_points(exp)[0]
	# By default we only ask for one point
	
	# Sample the point from our objective function, we can replace this with any function
	value_of_next_point = function_to_minimize(next_point_to_sample)
	
	print next_point_to_sample, value_of_next_point
	
	# Add the information about the point to the experiment historical data to inform the GP
	exp.historical_data.append_sample_points([SamplePoint(next_point_to_sample, value_of_next_point, 15.0)])
	# We can add some noise


while True:
	itterate()