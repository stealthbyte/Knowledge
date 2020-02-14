print(1 + 2)	  		    # 3\r\n
print(1, 2)		            # 1 2\r\n
try:
    print(1 + "a")		    # exception: unsupported operand…
except Exception as e:
    print(e)
print(1, "a")		        # 1 a\r\n
print(1, "a", end="") 	    # 1 a # no trailing CRLF
print()
print("o" "n" "e") 	        # one\r\n #parser concatenation
