class VehicleException(Exception):
    def __init__(self, msg):
        super(VehicleException, self).__init__(msg)
        self.msg = msg

    def __str__(self):
        return "Exception Occured: %s" % self.msg


class Vehicle(object):
    def __init__(self, top_speed, num_wheels, color):
        self.top_speed = top_speed
        self.num_wheels = num_wheels
        self.color = color
        self.miles_traveled = 0

    def __str__(self):
        msg = ("My %s vehicle has %d wheels, a top speed of %d, and has "
               "traveled %d miles" % (self.color,
                                      self.num_wheels,
                                      self.top_speed,
                                      self.miles_traveled))

        return msg

    def drive(self, hours):
        if hours < 0:
            raise VehicleException("Time driven (%s) can't be negative!" % hours)
        self.miles_traveled += (self.top_speed * hours)

    def honk(self):
        print("Hooooooooooooooonk")


class Motorcycle(Vehicle):
    def __init__(self, top_speed, color):  # overrides the parent class's constructor.
        super(Motorcycle, self).__init__(top_speed, 2, color)

    def honk(self):
        print("Beep Beep")


if __name__ == "__main__":
    my_bike = Motorcycle(10, "Red")
    print(my_bike)
    try:
        my_bike.drive(-1)
    except VehicleException as ve:
        print(ve)
