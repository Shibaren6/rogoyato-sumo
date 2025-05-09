import turtle as t
import math
import random

# Constants
RING_RADIUS = 200  # Radius of the circular floor
ROBOT_RADIUS = 20  # Radius of each robot
ATTACK_SPEED = 3   # Speed multiplier for our robot
WANDER_SPEED = 2   # Speed multiplier for enemy robot

# Initialize screen
screen = t.Screen()
screen.setup(width=600, height=600)
screen.tracer(0)  # Turn off animation for smoother updates

# Draw the circular floor
ring = t.Turtle()
ring.hideturtle()
ring.penup()
ring.goto(0, -RING_RADIUS)
ring.pendown()
ring.circle(RING_RADIUS)

# Create robots
our_robot = t.Turtle(shape="circle")
our_robot.color("blue")
our_robot.penup()
our_robot.goto(0, -175)  # Initial position of our robot
our_robot.mass = 5
our_robot.velocity = [0, 0]

enemy_robot = t.Turtle(shape="circle")
enemy_robot.color("red")
enemy_robot.penup()
enemy_robot.goto(0, 175)  # Initial position of enemy robot
enemy_robot.mass = 5
enemy_robot.velocity = [0, 0]

# Function to handle collisions using momentum laws
def handle_collision(robot1, robot2):
    dx = robot2.xcor() - robot1.xcor()
    dy = robot2.ycor() - robot1.ycor()
    distance = math.sqrt(dx**2 + dy**2)

    if distance <= 2 * ROBOT_RADIUS:
        normal = [dx / distance, dy / distance]
        tangent = [-normal[1], normal[0]]

        v1n = normal[0] * robot1.velocity[0] + normal[1] * robot1.velocity[1]
        v1t = tangent[0] * robot1.velocity[0] + tangent[1] * robot1.velocity[1]
        v2n = normal[0] * robot2.velocity[0] + normal[1] * robot2.velocity[1]
        v2t = tangent[0] * robot2.velocity[0] + tangent[1] * robot2.velocity[1]

        v1n, v2n = v2n, v1n

        robot1.velocity = [
            v1n * normal[0] + v1t * tangent[0],
            v1n * normal[1] + v1t * tangent[1],
        ]
        robot2.velocity = [
            v2n * normal[0] + v2t * tangent[0],
            v2n * normal[1] + v2t * tangent[1],
        ]

# Function to update robot positions
def update_positions():
    for robot in [our_robot, enemy_robot]:
        x = robot.xcor() + robot.velocity[0]
        y = robot.ycor() + robot.velocity[1]

        # Check if robot is out of bounds
        distance_from_center = math.sqrt(x**2 + y**2)
        if distance_from_center > RING_RADIUS:
            # Determine which robot is eliminated
            if robot == our_robot:
                display_result("Red wins!", "Blue gets pushed out by Red!")
            else:
                display_result("Blue wins!", "Red gets pushed out by Blue!")
            return  # Stop further updates

        # Check if robot is near the border and change direction
        if distance_from_center > RING_RADIUS - ROBOT_RADIUS:
            angle_to_center = math.atan2(-y, -x)
            robot.velocity[0] = math.cos(angle_to_center) * abs(robot.velocity[0])
            robot.velocity[1] = math.sin(angle_to_center) * abs(robot.velocity[1])

        # Update position
        robot.goto(x, y)

def display_result(winner_message, loser_message):
    """Display the result of the simulation and stop the game."""
    result_turtle = t.Turtle()
    result_turtle.hideturtle()
    result_turtle.penup()
    result_turtle.goto(0, 0)
    result_turtle.write(
        f"{winner_message}\n{loser_message}",
        align="center",
        font=("Arial", 16, "bold")
    )
    screen.update()
    t.done()  # Keep the window open after the simulation ends

# Function to implement attacking behavior
def attack_enemy():
    dx = enemy_robot.xcor() - our_robot.xcor()
    dy = enemy_robot.ycor() - our_robot.ycor()
    distance = math.sqrt(dx**2 + dy**2)

    if distance > 0:
        direction = [dx / distance, dy / distance]
        our_robot.velocity[0] = direction[0] * ATTACK_SPEED
        our_robot.velocity[1] = direction[1] * ATTACK_SPEED

# Function to make the enemy robot wander randomly
def wander_enemy():
    angle = random.uniform(0, 2 * math.pi)
    enemy_robot.velocity[0] = math.cos(angle) * WANDER_SPEED
    enemy_robot.velocity[1] = math.sin(angle) * WANDER_SPEED
    screen.ontimer(wander_enemy, 1000)  # Update direction every second

# Main simulation loop
def simulate():
    attack_enemy()  # Our robot attacks the enemy
    handle_collision(our_robot, enemy_robot)
    update_positions()
    screen.update()
    screen.ontimer(simulate, 20)

# Start wandering behavior for the enemy robot
wander_enemy()

# Start the simulation
simulate()
screen.mainloop()
