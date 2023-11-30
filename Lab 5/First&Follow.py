# Initialize dictionaries to store FIRST and FOLLOW sets
first_set = {}
follow_set = {}
non_terminals = set()
terminals = set()

# Function to compute FIRST set for a non-terminal
def compute_first_set(nt):
    if first_set.get(nt):
        return first_set[nt]

    first_set[nt] = set()
    for production in grammar_rules[nt]:
        for symbol in production:
            if symbol in terminals:
                first_set[nt].add(symbol)
                break
            elif symbol == '#':
                first_set[nt].add('#')
                break
            else:
                first_set[nt].update(compute_first_set(symbol))
                if '#' not in first_set[symbol]:
                    break
    return first_set[nt]

# Function to compute FOLLOW set for a non-terminal
def compute_follow_set(nt):
    if follow_set.get(nt):
        return follow_set[nt]

    follow_set[nt] = set()
    if nt == start_symbol:
        follow_set[nt].add('$')
    for non_term, productions in grammar_rules.items():
        for production in productions:
            for i, symbol in enumerate(production):
                if symbol == nt:
                    if i < len(production) - 1:
                        next_symbol = production[i + 1]
                        if next_symbol in terminals:
                            follow_set[nt].add(next_symbol)
                        else:
                            F_next = compute_first_set(next_symbol)
                            follow_set[nt].update(F_next.difference({'#'}))
                            if '#' in F_next:
                                follow_set[nt].update(compute_follow_set(non_term))
                    else:
                        follow_set[nt].update(compute_follow_set(non_term))
    return follow_set[nt]

try:
    print("Enter Details of LL1 Grammar. The entered grammar should be LL1.")
    terminal_count = int(input("Enter the number of terminals: "))
    print("Enter the terminal symbols:")
    terminals = set(input() for _ in range(terminal_count))
    non_terminal_count = int(input("Enter the number of non-terminals: "))
    print("Enter the non-terminal symbols:")
    non_terminals = set(input() for _ in range(non_terminal_count))
    start_symbol = input("Enter the starting symbol: ")
    production_count = int(input("Enter the number of productions: "))
    print("Enter the productions in the format NonTerminal -> Production1 | Production2 | ...")
    grammar_rules = {}
    for non_term in non_terminals:
        grammar_rules[non_term] = set()
    for _ in range(production_count):
        production_input = input()
        if '->' in production_input:
            non_term, productions = production_input.split('->')
            non_term = non_term.strip()
            productions = productions.split('|')
            grammar_rules[non_term] = grammar_rules[non_term].union([p.strip() for p in productions])
        else:
            print(f"Invalid production: {production_input}. Use 'NonTerminal -> Production1 | Production2' format.")
            continue

    # Compute FIRST and FOLLOW sets
    for non_term in non_terminals:
        compute_first_set(non_term)
        compute_follow_set(non_term)
    print("\nFIRST sets:")
    for non_term in non_terminals:
        print(f'FIRST({non_term}) = {sorted(list(first_set[non_term]))}')
    print("\nFOLLOW sets:")
    for non_term in non_terminals:
        print(f'FOLLOW({non_term}) = {sorted(list(follow_set[non_term]))}')
except Exception as e:
    print(f"An error occurred: {e}")
