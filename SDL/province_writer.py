number = 53
while True:
    name = input(number)
    desc = f'select = {number}; #{name}\nname = "{name};\nCON = 신라;\nOWN = 신라;'
    open(f'{name}.txt', mode='w', encoding='utf-8').write(desc)
    number += 1
