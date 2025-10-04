# generate the command
cmd = "|".join([f"add(Col{i},0,*)" for i in range(1001)])  # Col0 to Col1000

# write to a file
with open("command.txt", "w") as f:
    f.write(cmd)

print("Command written to command.txt ✅")

