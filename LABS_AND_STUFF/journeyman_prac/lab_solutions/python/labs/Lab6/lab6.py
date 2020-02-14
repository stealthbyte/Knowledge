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
        self.miles_traveled += (self.top_speed * hours)

    def honk(self):
        print("Hooooooooooooooonk")


class Motorcycle(Vehicle):
    def __init__(self, top_speed, color):  # overrides the parent class's constructor.
        super(Motorcycle, self).__init__(top_speed, 2, color)

    def honk(self):
        print("Beep Beep")


if __name__ == "__main__":
    my_car = Vehicle(120, 4, "Bluestone Metallic")
    print(my_car)
    my_car.honk()
    my_car.drive(2)
    print(my_car)

    my_delorean = Vehicle(top_speed=88, num_wheels=4, color="Stainless Steel")
    print(my_delorean)
    my_delorean.honk()
    my_delorean.drive(2)
    print(my_delorean)

    my_bike = Motorcycle(10, "Red")
    print(my_bike)
    my_bike.honk()
    my_bike.drive(1)
    print(my_bike)
