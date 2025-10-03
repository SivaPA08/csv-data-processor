import sys
import json

data = json.loads(sys.argv[1])
var_name = data["var_name"]
var_value = data["var_value"]
condition = data["condition"]

vars = {var_name: var_value}

# Evaluate and print 1 or 0
result = eval(condition, {}, vars)
print("1" if result else "0")

