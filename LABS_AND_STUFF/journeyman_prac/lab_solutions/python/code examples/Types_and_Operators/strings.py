ystring = "The worst of times"				# assignment
xstring = "The best" + " of times"			# concatenation
xstring = xstring * 3					    # repetition
xstring[0]						            # indexing
xstring[-1]          						# indexing from end
xstring[1:4]		        				# slicing
len(xstring)				        		# length method
if xstring < ystring: pass			        # comparison
if "rst" in ystring: pass				    # inclusion
xstring = xstring + "\r\nnewlines"		    # escaped control
xstring += 'single quotes' + """triple quotes""" + r"\nraw"	# raw
