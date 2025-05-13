state   = 'Basla'
yon = 0

left_speed  = 0
right_speed = 0

def turn():
    global left_speed, right_speed
    if(yon == 0):
        left_speed  = 60
        right_speed = -60
    else:
        left_speed  = -60
        right_speed = 60

def control(inputs):
    global yon
    global state
    global left_speed
    global right_speed

    front_left  = inputs["front-left"]
    front_right = inputs["front-right"]
    back_left  = inputs["back-left"]
    back_right = inputs["back-right"]
    dist_left  = inputs["distance-left"]
    dist_right = inputs["distance-right"]
    dist = inputs["distance-middle"]
    
    if(dist_left < 750):
        yon = 1
    
    if(dist_right < 750):
        yon = 0
            
    if state == 'Basla':
        state = "Ara";
            
    elif state == 'Ara':
        
        if front_left > 0.25 or front_right > 0.25 or back_left > 0.25 or back_right > 0.25:
            left_speed  = 0
            right_speed = 0
            turn()
        elif(dist < 300):
            state = "Saldir"
        else:
            left_speed  = 0
            right_speed = 0
            turn()
    
    if state == 'Saldir':
        t = dist/60
        while(t > 0):
            if front_left > 0.25 or front_right > 0.25 or back_left > 0.25 or back_right > 0.25:
                left_speed  = 0
                right_speed = 0
                state = "Ara"
            left_speed  = 60
            right_speed = 60
            
            if(dist == 0):
                state = "Ara"
            
            t -= 1
        
        state = "GeriCekil"
            
    elif state == 'GeriCekil':
        t = 5
        while(t > 0):
            if front_left > 0.25 or front_right > 0.25 or back_left > 0.25 or back_right > 0.25:
                left_speed  = 0
                right_speed = 0
                state = "Ara"
            left_speed  = -60
            right_speed = -60
            
            t -= 1
            
        state = "Ara"
    

    return {
        'leftSpeed':  left_speed,
        'rightSpeed': right_speed,
        'log': [
            { 'name': 'Left',  'value':   left_speed,    'min': -60, 'max': 60 },
            { 'name': 'Right', 'value':   right_speed,   'min': -60, 'max': 60 }
        ]
    }
