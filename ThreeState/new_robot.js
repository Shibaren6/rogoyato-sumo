const cuboid    = jscad.primitives.cuboid;
const cylinder  = jscad.primitives.cylinder;
const translate = jscad.transforms.translate;
const rotate    = jscad.transforms.rotate;
const union     = jscad.booleans.union;
const subtract  = jscad.booleans.subtract;

function robot() {
    var wedge = cuboid({ size: [20, 20, 20], center: [0, 5, -2] });
    wedge = translate([0, 10, 10 * Math.sqrt(2)+1], rotate([-Math.PI/4, 0, 0], wedge));
    var body = cuboid({ size: [15, 20, 9], center: [0, 0, 5] });
    body = subtract(body, wedge);
    
    var wheel_left = cylinder({ radius:4, height: 2, segments: 96});
    wheel_left = translate([-9, -6, 4], rotate([0, Math.PI/2, 0], wheel_left));
    wheel_left.material = 'rubber';

    var wheel_right = cylinder({ radius:4, height: 2, segments: 96});
    wheel_right = translate([ 9, -6, 4], rotate([0, Math.PI/2, 0], wheel_right));
    wheel_right.material = 'rubber';
    
    return {
        bodies:  [wheel_left, wheel_right, body],
        components: [
            {
                "type": "controller",
                "id": "control",
                "position": [0, -4, 8],
                "euler": [ 0, 0, 0, "XYZ" ]
            },
            {
                "type": "battery",
                "id": "battery",
                "position": [0, -1, 2],
                "euler": [ 0, 0, 0, "XYZ" ]
            },
            {
                "type": "motor",
                "id": "leftSpeed",
                "position": [-4, -6, 4],
                "euler": [ 0, -Math.PI/2, 0, "XYZ" ]
            },
            {
                "type": "motor",
                "id": "rightSpeed",
                "position": [4, -6, 4],
                "euler": [ 0, Math.PI/2, 0, "XYZ" ]
            },
            {
                "type": "distance",
                "id": "distance-left",
                "position": [-5.5, 2.75, 5.5],
                "euler": [ 0.05, 0, 3.1415 / 30  , "XYZ" ]
            },
            {
                "type": "distance",
                "id": "distance-right",
                "position": [ 5.5, 2.75, 5.5],
                "euler": [ 0.05, 0, -3.1415 / 30, "XYZ" ]
            },
            {
                "type": "ground",
                "id": "front-left",
                "position": [ -7, 6, 0.85]
            },
            {
                "type": "ground",
                "id": "front-right",
                "position": [  7, 6, 0.85]
            },
            {
                "type": "ground",
                "id": "back-left",
                "position": [ -7, -6, 0.85]
            },
            {
                "type": "ground",
                "id": "back-right",
                "position": [  7, -6, 0.85]
            }
        ]
    };
}
