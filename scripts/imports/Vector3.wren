class Vector3 {
    construct new() { 
        _x = 0
        _y = 0
        _z = 0
    }

    construct new(x_, y_, z_) { 
        _x = x_
        _y = y_
        _z = z_
    }

    x { _x }
    y { _y }
    z { _z }
    x=(rhs) { _x = rhs }
    y=(rhs) { _y = rhs }
    z=(rhs) { _z = rhs }

    -{
        return Vector3.new(-x, -y, -z)
    }

    +(rhs) {
        return Vector3.new(x + rhs.x, y + rhs.y, z + rhs.z)
    }

    -(rhs) {
        return Vector3.new(x - rhs.x, y - rhs.y, z - rhs.z)
    }

    *(rhs) {
        if (rhs is Vector3) {
            return Vector3.new(nx = x * rhs.x, ny = y * rhs.y, nz = z * rhs.z)
        } else {
            return Vector3.new(x * rhs, y * rhs, z * rhs)
        }
    }

    /(rhs) {
        if (rhs is Vector3) {
            return Vector3.new(x / rhs.x, y / rhs.y, z / rhs.z)
        } else {
            return Vector3.new(x / rhs, y / rhs, z / rhs)
        }
    }

    static magnitude(lhs) {
        return (lhs.x * lhs.x + lhs.y * lhs.y + lhs.z * lhs.z).pow(0.5)
    }

    magnitude() {
        return (x * x + y * y + z * z).pow(0.5)
    }

    static normalized(lhs) {
        if (lhs.magnitude() == 0) {
            return Vector3.new()
        }
        return lhs / magnitude()
    }

    normalized() {
        if (magnitude() == 0) {
            return Vector3.new()
        }
        return this / magnitude()
    }

    static inverse(lhs) {
        return -lhs
    }

    inverse() {
        return -this
    }

    static dot(lhs, rhs) {
        return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z)
    }

    dot(rhs) {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z)
    }

    static cross(lhs, rhs) {
        return Vector3.new(lhs.y * rhs.z - rhs.y * lhs.z, lhs.z * rhs.x - rhs.x * lhs.x, lhs.x * rhs.y - rhs.x * lhs.y)
    }

    cross(rhs) {
        return Vector3.new(y * rhs.z - rhs.y * z, z * rhs.x - rhs.x * x, x * rhs.y - rhs.x * y)
    }

    static toString(lhs) {
        return "(%(lhs.x), %(lhs.y), %(lhs.z))"
    }

    toString() {
        return "(%(x), %(y), %(z))"
    }
}