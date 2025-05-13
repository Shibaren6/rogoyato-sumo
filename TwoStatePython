state = "search"
search_speed = 15
straight_speed = 30
turn_speed = 25
iterations = 0
find_iteration = -1
transition = 50
last_known_enemy_direction = ""
search_rotation_direction = "right"

def control(inputs):
    global state, iterations, find_iteration, last_known_enemy_direction
    
    iterations += 1
    
    distance_right = inputs["distance-right"]
    distance_middle = inputs["distance-middle"]
    distance_left = inputs["distance-left"]
    
    front_left = inputs["front-left"]
    front_right = inputs["front-right"]
    back_left = inputs["back-left"]
    back_right = inputs["back-right"]
    
    left_speed = 0
    right_speed = 0
    
    speed = search_speed
    
    # Check if robot is near edge
    if front_left > 0.25 or front_right > 0.25 or back_left > 0.25 or back_right > 0.25:
        state = "search"
    
    # State machine for behavior selection
    if state == "search":
        speed = search_speed
        if distance_middle < 300:
            if find_iteration == -1:
                find_iteration = iterations
            elif iterations - find_iteration < transition:
                find_iteration = -1
                state = "attack"
                
    elif state == "attack":
        speed = straight_speed
        if distance_middle >= 300 and distance_left >= 300 and distance_right >= 300:
            state = "search"
    
    # Motor control based on current state
    if state == "search":
        if last_known_enemy_direction == "left":
            left_speed = -speed
            right_speed = speed
        else:  # Default to right if not specified
            left_speed = speed
            right_speed = -speed
            
    elif state == "attack":
        if distance_middle < 300 or (distance_right < 300 and distance_left < 300):
            left_speed = speed
            right_speed = speed
            last_known_enemy_direction = "forward"
            print(last_known_enemy_direction)
            
        if distance_right < 300:
            left_speed = turn_speed
            right_speed = speed
            last_known_enemy_direction = "right"
            
        elif distance_left < 300:
            left_speed = speed
            right_speed = turn_speed
            last_known_enemy_direction = "left"
            
        else:
            left_speed = speed
            right_speed = speed
            last_known_enemy_direction = ""
    
    return {
        "leftSpeed": left_speed,
        "rightSpeed": -right_speed,
        "log": [
            {"name": "Distance Left", "value": distance_left, "min": 0, "max": 300},
            {"name": "Distance Right", "value": distance_right, "min": 0, "max": 300},
            {"name": "Left", "value": left_speed, "min": -45, "max": 45},
            {"name": "Right", "value": right_speed, "min": -45, "max": 45}
        ]
    }
