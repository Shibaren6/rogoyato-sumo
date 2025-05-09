var state = "search";

function control(inputs) {
    
    var distance_right = inputs["distance-right"];
    var distance_middle = inputs["distance-middle"];
    var distance_left  = inputs["distance-left"];
    
    var front_left  = inputs["front-left"];
    var front_right = inputs["front-right"];
    var back_left  = inputs["back-left"];
    var back_right = inputs["back-right"];
    
    var left_speed, right_speed;
    
    var speed = 30;
    

    if (front_left > 0.25 || front_right > 0.25 || back_left > 0.25 || back_right > 0.25) {
        state  = "search";
    }

    switch (state) {
        case "search":
            speed = 30;
            if (distance_middle < 300) {
                state = "straight";
            }
            break;
        case "straight":
            speed = 30;
            if (distance_middle == 300 && distance_left == 300 && distance_right == 300) {
                state = "search";
            }
            break;
    }

    switch (state) {
        case "search":
            left_speed  =  speed;
            right_speed =  -speed;
            break;
        case "straight":
            left_speed  =   speed;
            right_speed =  speed;
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
