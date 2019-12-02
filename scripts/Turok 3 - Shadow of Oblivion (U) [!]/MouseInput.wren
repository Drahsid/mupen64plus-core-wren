// CONSTANTS DO NOT TOUCH
var RAD2DEG = 57.29578049
var DEG2RAD = 0.01745329
var TAU = Num.pi * 2
var HPI = Num.pi / 2
var MINA = -89.999998 * DEG2RAD
var MAXA = 89.999998 * DEG2RAD

var FPSMAX = 0
var FPS60 = 1
var FPS30 = 2
var FPS15 = 3
var FPS60SCALE = 2
var FPS30SCALE = 1
var FPS15SCALE = 0.5

// Input config; m_pitch is vertical sensitivity, m_yaw is horizontal sensitivity, m_mul scales both sensitivities
var M_PITCH = 0.0025
var M_YAW = 0.0025
var M_MUL = 1

// Self explanatory
var FPSMODE = FPS30
var FPSSCALE = FPS30SCALE

// Not implemented; ignore
var SCALETOFPS = false

// Time before mouse input begins; softlocks in the main menu
var TIMEBEFOREEXECUTE = 2500

// Userdata
var PITCH = 0
var YAW = 0

class mupen {
    // Get rdram methods from C
    foreign static rdramRead8(addr)
    foreign static rdramWrite8(addr, input)
    foreign static rdramRead16(addr)
    foreign static rdramWrite16(addr, input)
    foreign static rdramRead32(addr)
    foreign static rdramReadf32(addr)
    foreign static rdramWrite32(addr, input)
    foreign static rdramWritef32(addr, input)
    foreign static rdramReadBuffer(addr, length)
    foreign static rdramWriteBuffer(addr, buf)

    // Get rom methods from C
    foreign static romRead8(addr)
    foreign static romWrite8(addr, input)
    foreign static romRead16(addr)
    foreign static romWrite16(addr, input)
    foreign static romRead32(addr)
    foreign static romWrite32(addr, input)
    foreign static romReadBuffer(addr, length)

    // Impleneted these just for this
    foreign static getMouseX()
    foreign static getMouseY()

    // Currently broken; don't use
    foreign static osdMessage(msg)

    static init() {
        FPSSCALE = FPSSCALE * 60
    }

    static onTick(framecount) {
        romWrite32(0x0006532C, 0)
        romWrite32(0x00065344, 0)
        romWrite32(0x00065354, 0)

        rdramWrite8(0x8013DB1F, FPSMODE)
        rdramWrite8(0x8013D9E7, FPSMODE)
        rdramWrite8(0x80139393, FPSMODE)
        rdramWritef32(0x8013D8B0, FPSSCALE)

        // Unlocks secrets
        rdramWrite32(0x806CB01A, 0x504F)
        rdramWrite32(0x806CB01C, 0x504F)
        rdramWrite32(0x801659C6, 0xFFFF)
        rdramWrite32(0x811659C4, 0xFFFF)

        if (framecount > TIMEBEFOREEXECUTE) {
            PITCH = PITCH - (getMouseY() * M_PITCH * M_MUL)
            YAW = YAW + (getMouseX() * M_YAW * M_MUL)

            PITCH = PITCH > MAXA && MAXA || PITCH < MINA && MINA || PITCH
            YAW = YAW > TAU && YAW - TAU || YAW < -TAU && YAW + TAU || YAW

            rdramWritef32(0x80690240, PITCH)
            rdramWritef32(0x8068F4DC, YAW)
        }

        if (framecount % 48 == 0) {
            if (framecount < TIMEBEFOREEXECUTE) {
                var delta = TIMEBEFOREEXECUTE - framecount
                System.print("Waiting to execute... %(delta) frames before execution!")
            } else {
                System.print("Angles: rad; %(PITCH), %(YAW) deg; %(PITCH * RAD2DEG), %(YAW * RAD2DEG)")
            }
        }
    }
}