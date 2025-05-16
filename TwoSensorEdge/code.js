var search_speed = 10;
var straight_speed = 20;
var back_speed = 20;
var turn_speed = 15;

var direction = "left";
var started = false;
var searching = false;

var state = "start";

function control(inputs) {
    
    var right = inputs["distance-right"] < 70;
    var left  = inputs["distance-left"] < 70;
    
    var front_left  = inputs["front-left"] > 0.25;
    var front_right = inputs["front-right"] > 0.25;
    var back_left  = inputs["back-left"] > 0.25;
    var back_right = inputs["back-right"] > 0.25;
    
    var left_speed = 0, right_speed = 0;
    
    if (front_left || front_right)
    {
        if(!started){
            state = "start_search";
        }
        else if (!left && !right && !searching)
        {
            state = "back";
        }
        else if(!searching)
        {
            state = "search";
        }
        
        if(front_left){
            direction = "right";
        }
        else{
            direction = "left";
        }
    }
    else if (back_left || back_right)
    {
        if ((left || right) && !searching)
        {
            state = "attack";
        }
        else if (!searching)
        {
            state = "search";
        }
    }
    
    switch(state)
    {
        case "search":
            if (right && left) {
                state = "attack";
                left_speed  = straight_speed;
                right_speed = straight_speed;
                break;
            }
            left_speed  = search_speed;
            right_speed = -search_speed;
            break;  
        case "attack":
            if (!right || !left)
            {
                if (!right && !left)
                {
                    //console.log("loosy");
                    state = "search";
                }
                else if (!right)
                {
                    //console.log("left arc");
                    left_speed  =  turn_speed;
                    right_speed =  straight_speed;
                    direction = "left";
                }
                else
                {
                    //console.log("right arc");
                    left_speed  =  straight_speed;
                    right_speed =  turn_speed;
                    direction = "right";
                }
            }
            else
            {
                left_speed  =  straight_speed;
                right_speed =  straight_speed;
            }
            break;
        case "back":
            if (!front_left && !front_right)
            {
                state = "search";
            }
            left_speed  =  -back_speed;
            right_speed =  -back_speed;
            break;
            
        case "start":
            left_speed  =  -back_speed;
            right_speed =  -back_speed;
            break;
        case "start_search":
            searching = true;
            if(direction == "left"){
                left_speed  =  -search_speed;
                right_speed =  search_speed;
            }
            else{
                left_speed  =  search_speed;
                right_speed =  -search_speed;
            }
            
            if (right && left)
            {
                searching = false;
                state = "attack";
                left_speed  =  straight_speed;
                right_speed =  straight_speed;
                started = true;
            }
            break;
    }
    
    if (left_speed == 0)
        console.log("loyn");

    return {
        leftSpeed: left_speed,
        rightSpeed: -right_speed,
        log: [
            {name: "Distance Left",  value: inputs["distance-left"], min: 0, max: 300},
            {name: "Distance Right", value: inputs["distance-right"], min: 0, max: 300},
            {name: "Left",  value: left_speed, min: -45, max: 45},
            {name: "Right", value: right_speed, min: -45, max: 45}
        ]
    };
}
