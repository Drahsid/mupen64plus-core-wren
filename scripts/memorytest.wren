class mupen {
    foreign static rdramRead8(addr)
    foreign static rdramWrite8(addr, input)
    foreign static rdramRead16(addr)
    foreign static rdramWrite16(addr, input)
    foreign static rdramRead32(addr)
    foreign static rdramWrite32(addr, input)
    foreign static rdramReadBuffer(addr, length)

    foreign static romRead8(addr)
    foreign static romWrite8(addr, input)
    foreign static romRead16(addr)
    foreign static romWrite16(addr, input)
    foreign static romRead32(addr)
    foreign static romWrite32(addr, input)
    foreign static romReadBuffer(addr, length)

    static onTick(framecount) {
        System.print("Hello, world! from onTick! Framecount: %(framecount)")
        var ram_r = rdramRead8(0x801C6FA1)
        rdramWrite8(0x801C6FA1, 1)
        System.print("RDRAM Read Test Result: %(ram_r)")
    }
}

