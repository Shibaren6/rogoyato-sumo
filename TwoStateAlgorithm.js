var state = "search";
var search_speed = 15;
var straight_speed = 30;
var turn_speed = 25;

var iterations = 0;
var find_iteration = -1;
var transition = 50;

var last_known_enemy_direction = "";
var search_rotation_direction = "right";

function control(inputs) {
    iterations++;
    var distance_right = inputs["distance-right"];
    var distance_middle = inputs["distance-middle"];
    var distance_left  = inputs["distance-left"];
    
    var front_left  = inputs["front-left"];
    var front_right = inputs["front-right"];
    var back_left  = inputs["back-left"];
    var back_right = inputs["back-right"];
    
    var left_speed, right_speed;
    
    var speed = search_speed;
    
    if (front_left > 0.25 || front_right > 0.25 || back_left > 0.25 || back_right > 0.25) {
        state  = "search";
    }
    

    switch (state) {
        case "search":
            speed = search_speed;
            if (distance_middle < 300) {
                if (find_iteration == -1) {
                    find_iteration = iterations;
                }
                else if (find_iteration - iterations < transition) {
                    find_iteration = -1;
                    state = "attack";
                }
            }
            break;
        case "attack":
            speed = straight_speed;
            if (distance_middle >= 300 && distance_left >= 300 && distance_right >= 300) {
                state = "search";
            }
            break;
    }

    switch (state) {
        case "search":
            switch (last_known_enemy_direction) {
                default: 
                case "left":
                    left_speed  = -speed;
                    right_speed = speed;
                    break;
                case "right":
                    left_speed  = speed;
                    right_speed = -speed;
                    break;
            }
            break;
        case "attack":
            
            if (distance_middle < 300 || (distance_right < 300 && distance_left < 300)) {
                left_speed  = speed;
                right_speed = speed;
                
                last_known_enemy_direction = "forward"; //
                console.log(last_known_enemy_direction)
            }
            if (distance_right < 300) {
                left_speed  = turn_speed;
                right_speed = speed;
                
                last_known_enemy_direction = "right";
            }
            else if (distance_left < 300) {
                left_speed  = speed;
                right_speed = turn_speed;
                
                last_known_enemy_direction = "left";
            }
            else {
                left_speed  = speed;
                right_speed = speed;
                
                last_known_enemy_direction = "";
            }
            break;
    }

    return {
        leftSpeed: left_speed,
        rightSpeed: -right_speed,
        log: [
            {name: "Distance Left",  value:  distance_left, min: 0, max: 300},
            {name: "Distance Right", value: distance_right, min: 0, max: 300},
            {name: "Left",  value: left_speed, min: -45, max: 45},
            {name: "Right", value: right_speed, min: -45, max: 45}
        ]
    };
}
