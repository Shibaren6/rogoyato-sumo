# --- Parametreler ---
TURN_SPEED       = 30
# 270 Derece Dönmek İçin Gereken Counter = 46(Yaklaşık 270 Biraz Daha Büyük)

# --- FSM State kayıtları ---
state       = 'search'   # 'search', '270', 'attack'
counter     = 0
direction   = 'left'       # 'left' veya 'right'


steps270 = 0 # State İçindeki Aşamalar İçin
last_direction = 'left'

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
    global fl, fr, dm, dl, dr, state, counter, direction, steps270, last_direction
    
    fl = inputs["front-left"]
    fr = inputs["front-right"]
    dm = inputs["distance-middle"]
    dl = inputs["distance-left"]
    dr = inputs["distance-right"]
    
    
    if (fl > 0.25 or fr > 0.25):
        state = "search"
        steps270 = 0
        counter = 0    
    
    if (dl < 300 or dr < 300):
        if (dl < 300 and dr < 300):
            direction = 'straight'
        elif dl < 300:
            direction = 'left'
        elif dr < 300:
            direction = 'right'
        last_direction = direction
                
        
    if state == "search":
        if dm < 300:
            state = '270'
        else:
            if last_direction == 'left':
            	left_speed  = -TURN_SPEED
            	right_speed = TURN_SPEED
            else:
                left_speed  = TURN_SPEED
                right_speed = -TURN_SPEED
    elif state == "270":
        pass # devam edilecek
    elif state == "attack":
        if dm < 300:
            left_speed  = TURN_SPEED
            right_speed = TURN_SPEED

    return {
        'leftSpeed':  left_speed,
        'rightSpeed': -right_speed,
        'log': [
            {'name': 'Left',  'value': left_speed,  'min': -45, 'max': 45},
            {'name': 'Right', 'value': right_speed, 'min': -45, 'max': 45}
        ]
    }
