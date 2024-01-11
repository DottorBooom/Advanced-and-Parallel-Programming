# Davide Martinelli SM3201226

#########################################################
#                      Exception                        #
#########################################################

# An approach of handling exceptions at the outermost level has been chosen. 
# In short, when an error is handled, it results in a series of 1, 2, or 3 cascading exceptions, 
# allowing, during debugging, to track the program's execution and what it was processing.

# Exception for an empty stack.
class EmptyStackException(Exception):
    pass

# Exception for when a variable has not been initialized in the environment.
class MissingVariableException(Exception):
    pass

# Exception for when a variable has been initialized in the environment but without a value.
class MissingVariableValueException(Exception):
    pass

#########################################################
#                     Program body                      #
#########################################################

# Class for the stack
class Stack:

    def __init__(self): # Construction of an empty stack
        self.data = []

    def push(self, x):  # Adds elements to the stack (at the end)
        self.data.append(x) 

    def pop(self):      # Extracts an element from the stack
        if self.data == []:  # If there are no other elements, then I throw an Empty Stack exception.
            raise EmptyStackException
        res = self.data[-1]  # Otherwise, I take the last element (tail).
        self.data = self.data[0:-1] # The content of the stack now includes only elements from 0 to i.
        return res # Return the element extracted from the stack.

    def is_empty(self):
        if len(self.data) != 0:
            return False
        else:
            return True

    def __str__(self): 
        return " ".join([str(s) for s in self.data])

# Class that transforms the string into one or more objects that will then be executed.
class Expression:

    # Initialize the stack.
    def __init__(self):
        self.expression_stack = Stack()

    @classmethod
    def from_program(cls, text, dispatch):
        expr = cls()

        # Check the correctines of the parameters.
        if type(text) is not str:
            print(f"The parameter text was not a str object, value given {text}")
            return None

        if type(dispatch) is not dict:
            print(f"The parametre dispatch was not a dict object, value given {dispatch}")
            return None

        # Split the string into a list of words using " ".
        split_expression = text.split(" ") 
        # Iterate through the list.
        for i in split_expression:
            
            if i == "":
                print(f"Error in the spacing of the string, text splitted {split_expression}")
                return None

            try:
                # If I can turn that word into an integer, I save it as a constant class.
                if  i.isdigit():
                    saved = Constant(int(i))
                else:
                    saved = Constant(float(i))
            except:
                # If it's a string not convertible to a number, I check if it's a known operation.
                if i in dispatch.keys(): # If it's in the dictionary.
                    args = [] # I initialize the list to be passed as a parameter.
                    current_operation = dispatch[i] # Let's take the class of that keyword from the dictionary           
                    K = current_operation.arity # I save the arity of that class.

                    try: # I enter a try block because the elements might not be enough.
                        for i in range(K): # For as many times as it requires elements.
                            element = expr.expression_stack.pop() # I remove them from the stack.
                            args.append(element) # And I put them in the list.
                        
                        # I can then push the result of that operation onto the stack.
                        saved = current_operation(args) # I simply pass the list of elements as a parameter.
                        
                    except EmptyStackException:
                        print(f"Missing parametre during the computation of {current_operation}")
                        return None
                    
                elif i[0:4] == "prog":
                    args = []
                    current_operation = Prog
                    K = int(i[4:])

                    try: # I enter a try block because the elements might not be enough.
                        for i in range(K): # For as many times as it requires elements.
                            element = expr.expression_stack.pop() # I remove them from the stack.
                            args.append(element) # And I put them in the list.
                        
                        # I can then push the result of that operation onto the stack.
                        saved = current_operation(args) # I simply pass the list of elements as a parameter.
                        
                    except EmptyStackException:
                        print(f"Missing parametre during the computation of {current_operation}")
                        return None
                else:
                    # If the string is not an operation, then it is a variable.
                    saved = Variable(i)
        
            expr.expression_stack.push(saved) # I insert it into the stack.
            # In this way, I will read my operations in reverse, but the last operation...
            # ...that I will read will be the first to be removed from the stack.
    
        return expr # I return this instance that I have modified.

    def evaluate(self, env):

        # I base everything on recursive cascade. I will execute the evaluate function on all elements of the stack,
        # and, if the last operation returns a value, I make it return to the main function in turn.
        # Another important thing is that once all elements are popped, you also need to push.
        # Otherwise, the class becomes unusable, and every time I have to do the string from the program.
        # If the intention is, however, precisely this, just remove the push from the for loop.

        if type(env) is not dict:
            print("A non-dictionary object was passed as parameter")
            return None
        value = 0
        elem = []
        i = None
        try:
            while not self.expression_stack.is_empty():
                i = self.expression_stack.pop()
                elem.append(i)
            for i in elem:
                value = i.evaluate(env)
                self.expression_stack.push(i)
            return value
        except MissingVariableException:
            print(f"A variable was missing during the execution of {i}\nCheck if prog was used correctly or if set/alloc wasn't used")
            return None
        except MissingVariableValueException:
            print(f"A variable value was missing during the execution of {i}\nCheck if all the values given were initialized")
        
    
    def __str__(self):

        # The __str__ def work as the same as evaluate. Excpete, it print the code.

        value = ""
        elem = []
        while not self.expression_stack.is_empty():
            i = self.expression_stack.pop()
            elem.append(i)
        for i in elem:
            value += i.__str__() + "\n"
            self.expression_stack.push(i)
        return value

# Class that associates words that are not operations or integers with variables.
class Variable(Expression):

    def __init__(self, name):
        self.name = name

    def evaluate(self, env):
        if self.name not in env:
            raise MissingVariableException
        elif env[self.name] == None:
            raise MissingVariableValueException
        else:
            # If I have reached this point, both the variable and the value are present. I return the value of this variable.
            return env[self.name]

    def __str__(self):
        return self.name

# Class that saves integers into an object.
class Constant(Expression):

    def __init__(self, value):
        self.value = value

    def evaluate(self, env):
        return self.value

    def __str__(self):
        return str(self.value)

#########################################################
#                      Operation                        #
#########################################################

# Parent class that will allow the implementation of more specific operations.
class Operation(Expression):

    def __init__(self, args):
        self.args = args # I save the list of arguments.

    def evaluate(self, env):
        # An evaluation of the operation occurs with the operation between
        # the two parts. However, it depends on whether they are directly accessible.
        return self.op(self.args, env) # I use the concept of cascading operations and execute them on the evaluate of the subclass.

    def op(self, *args):
        raise NotImplementedError()
        # It will be left to the specific subclasses.

    def __str__(self):
        pass

class NonaryOp(Operation):
    arity = 0 # arità della classe

class UnaryOp(Operation):
    arity = 1 # arità della classe

class BinaryOp(Operation):
    arity = 2

class TernaryOp(Operation):
    arity = 3

class QuaternaryOp(Operation):
    arity = 4

# Each of these classes assumes that errors have already been handled externally, 
# except for errors related to specific operations, which will be handled by internal Python exceptions. 
# I don't think I need to comment on the classes because they are very simple and intuitive, 
# and they perfectly follow the explanation provided in the PDF.

class Addition(BinaryOp):
    
    def op(self, args, env):
        
        return args[0].evaluate(env) + args[1].evaluate(env)
    
    def __str__(self):
        return f"({self.args[0].__str__()} + {self.args[1].__str__()})"

class Subtraction(BinaryOp):

    def op(self, args, env):
        
        return args[0].evaluate(env) - args[1].evaluate(env)
    
    def __str__(self):
        return f"({self.args[0].__str__()} - {self.args[1].__str__()})"

class Division(BinaryOp):
    
    def op(self, args, env):
        
        return args[0].evaluate(env) / args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} / {self.args[1].__str__()})"

class Multiplication(BinaryOp):

    def op(self, args, env):
        
        return args[0].evaluate(env) * args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} * {self.args[1].__str__()})"

class Modulus(BinaryOp):
    
    def op(self, args, env):
        return args[0].evaluate(env) % args[1].evaluate(env)

    def __str__(self):
        return f"({self.args[0].__str__()} % {self.args[1].__str__()})"

class Reciprocal(UnaryOp):
    
    def op(self, args, env):
        return 1/args[0].evaluate(env)

    def __str__(self):
        return f"(1/{self.args[0].__str__()})"

class AbsoluteValue(UnaryOp):
    
    def op(self, args, env):
        return abs(args[0].evaluate(env))

    def __str__(self):
        return f"|{self.args[0].__str__()}|"

class Power(BinaryOp):
    
    def op(self, args, env):
        return args[0].evaluate(env)**args[1].evaluate(env)

    def __str__(self):
        return f"pow({self.args[0].__str__()},{self.args[1].__str__()})"

class GratherThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) > args[1].evaluate(env):
            return 1
        return 0

    def __str__(self):
        return f"({self.args[0].__str__()} > {self.args[1].__str__()})"

class GratherEqualThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) >= args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} >= {self.args[1].__str__()})"
    
class LessThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) < args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} < {self.args[1].__str__()})"

class LessEqualThan(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) <= args[1].evaluate(env):
            return 1
        return 0

    def __str__(self):
        return f"({self.args[0].__str__()} <= {self.args[1].__str__()})"

class Equal(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) == args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} == {self.args[1].__str__()})"

class NotEqual(BinaryOp):

    def op(self, args, env):
        if args[0].evaluate(env) != args[1].evaluate(env):
            return 1
        return 0
    
    def __str__(self):
        return f"({self.args[0].__str__()} != {self.args[1].__str__()})"

class Print(UnaryOp):

    def op(self, args, env):
        x = args[0].evaluate(env)
        print(x)    
        return x 
    
    def __str__(self):
        return f"print {self.args[0].__str__()}"

class IF(TernaryOp):

    def op(self, args, env):
        if args[0].evaluate(env):
            return args[1].evaluate(env)
        else:
            return args[2].evaluate(env)

    def __str__(self):
        return f"if {self.args[0].__str__()}\n{self.args[1].__str__()}\nelse\n{self.args[2].__str__()}"

class While(BinaryOp):
    
    def op(self, args, env):
        while args[0].evaluate(env):
            args[1].evaluate(env)

    def __str__(self):
        return "while"+self.args[0].__str__()+"\n{\n"+self.args[1].__str__()+"\n}"

class For(QuaternaryOp):
    
    def op(self, args, env):
        if args[0] not in env:
            env[args[0]] = 0 
        elif env[args[0]] == None:
            raise MissingVariableException

        for i in range(args[1].evaluate(env),args[2].evaluate(env)):
            env[args[0].__str__()] = i
            args[3].evaluate(env)

    def __str__(self):
        return "for "+self.args[0].__str__()+" in range("+self.args[1].__str__()+","+self.args[2].__str__()+")\n{\n"+self.args[3].__str__()+"\n}"

class Setq(BinaryOp):

    def op(self, args, env):
        env[args[0].__str__()] = args[1].evaluate(env)
        return env[args[0].__str__()]

    def __str__(self):
        return f"set {self.args[0].__str__()} = {self.args[1].__str__()}"

class Setv(TernaryOp):

    def op(self, args, env):
        env[args[0].__str__()][args[1].evaluate(env)] = args[2].evaluate(env)
        return env[args[0].__str__()][args[1].evaluate(env)]

    def __str__(self):
        return f"set {self.args[0].__str__()}[{self.args[1].__str__()}] = {self.args[2].__str__()}"

class Alloc(UnaryOp):

    def op(self, args, env):
        env[args[0].__str__()] = 0

    def __str__(self):
        return f"alloc {self.args[0].__str__()} = 0"

class vAlloc(BinaryOp):
    
    def op(self, args, env):
        env[args[0].__str__()] = [0] * args[1].evaluate(env)

    def __str__(self):
        return f"valloc {self.args[0].__str__()}[{self.args[1].__str__()}] = [0]"

class Prog(Operation):

    def op(self, args, env):
        for i in range(len(args)-1):
            args[i].evaluate(env)
        return args[-1].evaluate(env)
    
    def __str__(self):
        str = "prog4\n{"
        for i in range(len(self.args)):
            str += "\n" + self.args[i].__str__()
        str += "\n}"
        return str

class Nop(NonaryOp):

    def op(self, args, env):
        pass

    def __str__(self):
        return "nop"   

class DefSub(BinaryOp):

    def op(self, args, env):
        if args[0] not in env:
            env[args[0].__str__()] = args[1].evaluate
        elif env[args[0]] == None:
            raise MissingVariableException 
        else:
            env[args[0].__str__()] = args[1].evaluate

    def __str__(self):
        return f"defsub {self.args[0].__str__()} = ({self.args[1].__str__()})"   

class Call(UnaryOp):

    def op(self, args, env):
        if args[0].__str__() not in env:
            raise MissingVariableException 
        elif env[args[0].__str__()] == None:
            raise MissingVariableException
        else:
            return env[args[0].__str__()](env)

    def __str__(self):
        return f"call {self.args[0].__str__()}"

#########################################################
#                      Main body                        #
#########################################################

def main():
    d = {"+" : Addition, "*" : Multiplication, "-" : Subtraction, "/" : Division, "%" : Modulus, "1/": Reciprocal, "abs": AbsoluteValue, "**": Power, 
        ">" : GratherThan, ">=" : GratherEqualThan, "<" : LessThan, "<=" : LessEqualThan, "=" : Equal, "!=" : NotEqual, 
        "if" : IF, "while" : While, "for" : For, "setq" : Setq, "setv" : Setv, "alloc" : Alloc, "valloc" : vAlloc, 
        "print" : Print, "nop" : Nop, "defsub" : DefSub, "call" : Call}

    # Change this string for a different program
    example = "num1 print num2 num1 % num2 setq hold num1 setq num1 hold setq prog4 0 num2 != while hold alloc 6 num2 setq 12 num1 setq prog4"
    # Change this dictionary for manually insert value in the expression
    values = {}

    e = Expression.from_program(example, d)
    if e is None: # If it's none, it means something went wront during the computation of the string
        return
    print(e)
    res = e.evaluate(values) 
    print(f"The return value is : {res}")

if __name__ == "__main__":
    main() 