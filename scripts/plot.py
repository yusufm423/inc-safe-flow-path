import csv
import matplotlib.pyplot as plt

with open('sta6.csv', newline='') as csvfile:
    # Create a CSV reader object
    reader = csv.reader(csvfile)
    
    # Retrieve the rows and columns of interest
    x_orange = [[float(cell) for cell in row[0:100]] for row in reader][20]
# x_orange.sort()

# Open the CSV file
with open('dyn6.csv', newline='') as csvfile:
    # Create a CSV reader object
    reader = csv.reader(csvfile)
    
    # Retrieve the rows and columns of interest
    x_blue = [[float(cell) for cell in row[0:100]] for row in reader][20]
# x_blue.sort()

# Open the CSV file
with open('inter6.csv', newline='') as csvfile:
    # Create a CSV reader object
    reader = csv.reader(csvfile)
    
    # Retrieve the rows and columns of interest
    x_green = [[float(cell) for cell in row[0:100]] for row in reader][20]
# x_green.sort()

# Plot the list of elements
plt.plot(x_orange, color='orange')
plt.plot(x_blue, color='blue')
plt.plot(x_green, color='green')

# Adding title and axis labels
plt.title('Runtime: Blue=dynamic and Orange Plot=static')
plt.xlabel('graph number')
plt.ylabel('microseconds')

# Displaying the plot
plt.show()
