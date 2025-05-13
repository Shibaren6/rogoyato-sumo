import time

# --- Parametreler ---
TURN_SPEED       = 30

# --- FSM State kayıtları ---
state       = 'ara'   # 'ara', 'ilerle', 'don'
counter     = 0
direction   = 'left'       # 'left' veya 'right'

left_speed = 0
right_speed = 0

fl = 0
fr = 0
dm = 0
dl = 0
dr = 0

current = -1
    
def control(inputs):
    """
    1) transition ile state güncellenir
    2) o state'in act_...() fonksiyonu çalıştırılır
    3) komut döndürülür
    """
    global fl, fr, dm, dl, dr, state, counter, direction, current
    
    fl = inputs["front-left"]
    fr = inputs["front-right"]
    dm = inputs["distance-middle"]
    dl = inputs["distance-left"]
    dr = inputs["distance-right"]
    
    if dl < 300:
        direction = 'left'
    
    if dr < 300:
        direction = 'right'
    
    if fl or fr:
        counter = 5
        state = 'don'
    
    if state == 'ara':
        if (dl < 50 and dr < 50) or dm < 300:
            current = time.time() if current == -1 else current
            if(time.time() - current > 0.1):
                current = -1
                state = 'ilerle'
        else:
            counter = 3
            state = 'don'
    elif state == 'ilerle':
            left_speed  = TURN_SPEED
            right_speed = TURN_SPEED
            
            if dm >= 300:
                state = 'ara'
    elif state == 'don':
        if direction !='left':
            left_speed  = TURN_SPEED
            right_speed = -TURN_SPEED
            counter -= 1
        else:
            left_speed  = -TURN_SPEED
            right_speed = TURN_SPEED
            counter -= 1
        
        if (counter <= 0):
            state = 'ara'
    else:
        state = 'ara'

    return {
        'leftSpeed':  left_speed,
        'rightSpeed': -right_speed,
        'log': [
            {'name': 'Left',  'value': left_speed,  'min': -45, 'max': 45},
            {'name': 'Right', 'value': right_speed, 'min': -45, 'max': 45}
        ]
    }
